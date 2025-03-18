#include "config/config.h"

#include <gtest/gtest.h>

TEST(ConfigTest, HandlesConfigConstructorWithoutPath) {
  config::Config cfg;
  auto fat_cfg = cfg.get_fat_config();
  EXPECT_EQ(fat_cfg.segment_number, 15);

  auto info_cfg = cfg.get_info_config();
  EXPECT_EQ(info_cfg.owner_name, "Owner");
  EXPECT_EQ(info_cfg.volume_id, "Volume");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}