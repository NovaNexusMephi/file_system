FS_EXEC=../build/src/dialog/FileSystemModel

CMD_DIR=commands
EXP_DIR=expectations
LOG_DIR=logs

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

compare_logs() {
    local result_log="$1"
    local expected_log="$2"

    if diff --strip-trailing-cr --ignore-space-change "$result_log" "$expected_log" > /dev/null 2>&1; then
        echo -e "${GREEN}Логи совпадают: $expected_log${NC}"
        return 0
    else
        echo -e "${RED}Ошибка: Логи не совпадают для $expected_log${NC}"
        echo -e "${RED}Ожидаемый результат:${NC}"
        cat "$expected_log"
        echo -e "${RED}Полученный результат:${NC}"
        cat "$result_log"
        exit 1
    fi
}

for cmd_file in "$CMD_DIR"/*.txt; do
    # Получаем имя без пути и расширения
    base_name=$(basename "$cmd_file" .txt)
    expect_file="$EXP_DIR/expect_${base_name}.log"

    echo "Запускаю тест: $cmd_file"

    # Проверяем наличие ожидаемого файла
    if [ ! -f "$expect_file" ]; then
        echo -e "${RED}Ошибка: Не найден ожидаемый файл $expect_file${NC}"
        exit 1
    fi

    # Удаляем старые логи, если есть
    rm -rf "$LOG_DIR"

    # Запускаем файловую систему
    cat "$cmd_file" | "$FS_EXEC"

    # Ждём появления логов (максимум 10 секунд)
    for i in {1..10}; do
        sleep 1
        if [ -d "$LOG_DIR" ]; then
            break
        fi
    done

    # Проверяем, что появилась папка сессии
    session_dir=$(find "$LOG_DIR" -mindepth 1 -maxdepth 1 -type d | head -n 1)
    if [ -z "$session_dir" ]; then
        echo -e "${RED}Ошибка: Не найдена папка сессии в $LOG_DIR${NC}"
        exit 1
    fi

    result_log="$session_dir/results.log"

    if [ ! -f "$result_log" ]; then
        echo -e "${RED}Ошибка: Не найден файл result.log в $session_dir${NC}"
        exit 1
    fi

    # Сравниваем логи
    compare_logs "$result_log" "$expect_file"

    # Удаляем логи перед следующим запуском
    rm -rf "$LOG_DIR"
done

echo -e "${GREEN}Все тесты успешно пройдены!${NC}"