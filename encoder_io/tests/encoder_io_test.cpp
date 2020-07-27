#include "../encoder_io.h"
#include <array>
#include <gtest/gtest.h>

TEST(encoder_open, encoder_open)
{
	EXPECT_THROW(encoder_io enc("no_port"), std::runtime_error);
	EXPECT_NO_THROW(encoder_io enc("/dev/ttyACM0"));
}
TEST(encoder_read, read_all)
{
	encoder_io encoder("/dev/ttyACM0");

	std::array<int, 6> x = encoder.encoder_io::encoder_values();
	for (int i = 0; i < 6; i++)
		EXPECT_TRUE(0 <= x[i] && x[i] <= 200) << "#" << i << " Encoder value = " << x[i] << " is not within valid range 0 - 200";
}
TEST(encoder_read, read_1)
{
	encoder_io encoder("/dev/ttyACM0");

	int x = encoder.encoder_io::encoder_values(2);
	EXPECT_TRUE(0 <= x && x <= 200) << "#3 Encoder value = " << x << " is not within valid range 0 - 200";
}

TEST(encoder_write, write_reset)
{
	encoder_io encoder("/dev/ttyACM0");
	encoder.encoder_io::encoder_write('R');
	std::array<int, 6> x = encoder.encoder_io::encoder_values();
	for (int i = 0; i < 6; i++)
		EXPECT_TRUE(0 == x[i]) << "Encoders were not Reset" << i;
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}