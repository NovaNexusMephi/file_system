#include "filesystem/catalog.hpp"

#include <format>

namespace filesystem {

// Можно хранить количество свободных блоков и количесвто блоков, которые доступны для последовательной записи.

// Алгоритм:

// 1. Проверяем есть ли свободные записи (считать ли free свободной записью(наверное да)) -> throw
// 2. Проверяем есть ли свободные блоки (вообще все) -> throw
// 3. Проверяем есть ли свободные блоки для последовательной записи и записи в последнем сегменте (для эффективности,
// чтобы сделать простой пуш)
// 4. Ищем первую запись free нужного размера (вариант с хеш мапой (а лучше просто с мапой) - запоминать где пусто) ->
// throw
// 5. Добавляем/меняем запись

// ! Права !

void Catalog::create(const std::string& filename, size_t size) {
    if (size > free_space_) {
        throw std::runtime_error("There is no free space in the catalog");
    }
    if (free_records_ == 0) {
        throw std::runtime_error("There are no free records in the catalog");
    }

    if (header_.counter_ < header_.count_) {
        segments_[header_.counter_].addRecord(filename, size);
    }
    for (auto& segment : segments_) {
        for (const auto& record : segment.getRecords()) {
            if (record.getType() == FileType::FREE) {
            }
        }
    }
}

// ! Права !

void Catalog::remove(const std::string& filename) {
    auto record = findRecord(filename);
    if (record) {
        record->setType(FileType::FREE);
        ++free_records_;
        free_space_ += record->getSize();
        return;
    }
    throw std::runtime_error("File not found");
}

// ! Права !

void Catalog::rename(const std::string& old_filename, const std::string& new_filename) {
    auto record = findRecord(old_filename);
    if (record) {
        record->setFilename(new_filename);
        return;
    }
    throw std::runtime_error("File not found");
}

// ! Права !

void Catalog::copy(const std::string& filename, const std::string& dist_filename) {
    auto record = findRecord(filename);
    if (record) {
        create(dist_filename, record->getSize());
        return;
    }
    throw std::runtime_error("File not found");
}

// ! Права !

void Catalog::move(const std::string& filename, const std::string& dist_filename) {
    auto record = findRecord(filename);
    if (record) {
        create(dist_filename, record->getSize());
        remove(filename);
        return;
    }
    throw std::runtime_error("File not found");
}

// ! Права !
// Возможно переводит запись в режим TEMPORARY
// А если он уже открыт

bool Catalog::open(const std::string& filename) {
    auto record = findRecord(filename);
    if (record) {
        return true;
    }
    return false;
}

// ! Права !

// Возможно обновляем дату последнего доступа
// Возможно переводит запись в режим PERMANENT
// А если файл и так закрыт

void Catalog::close(const std::string& filename) {
    auto record = findRecord(filename);
    if (record) {
        record->setType(FileType::PERMANENT);  // ..?
    }
}

// Как работать с write и read?????
// Сколько занимает блок????

std::optional<FileRecord> Catalog::findRecord(const std::string& filename) const {
    for (const auto& segment : segments_) {
        for (const auto& record : segment.getRecords()) {
            if (record.getType() != FileType::FREE && record.getFilename() == filename) {
                return {record};
            }
        }
    }
    return std::nullopt;
}

// ! Права !

std::vector<std::string> Catalog::dir() const {
    std::vector<std::string> result;
    std::string temp;
    for (const auto& segment : segments_) {
        for (const auto& record : segment.getRecords()) {
            if (record.getType() != FileType::FREE) {
                temp = std::format("{} {} {}", record.getTimestamp(), record.getSize(), record.getFilename());
                result.push_back(temp);
            }
        }
    }
    return result;
}

}  // namespace filesystem