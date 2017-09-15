#include "../conversions.hpp"
#include "tests-main.hpp"

using namespace conversions;

// TODO numbers randomly chosen, can be automated
TEST_CASE("Conversions", "[conversions]") {
    GIVEN("some starting parameters") {
        int expectedsize;
        string expectedstring;
        WHEN("Chars are removed") {
            string input = "super.M..a...n.....!........";
            removeChars(&input, ".");
            expectedstring = "superMan!";
            THEN("The retrieved equals expected") {
                REQUIRE(input == expectedstring);
            };
        }
        WHEN("Non-Existent Chars are removed") {
            string input = "super.M..a...n.....!........";
            removeChars(&input, ".....");
            expectedstring = "super.M..a...n!...";
            THEN("The retrieved equals expected") {
                REQUIRE(input == expectedstring);
            };
        }
        WHEN("Decimal converted to hex") {
            int input1 = 3146542;
            unsigned int input2 = 3146542;
            string result1 = decToHex(input1);
            string result2 = decToHex(input2);
            expectedstring = "2e0330";
            THEN("Hex equals expected hex") {
                REQUIRE(expectedstring == result1);
                REQUIRE(expectedstring == result2);
            };
        }
        WHEN("Hex is reversed") {
            string input1 = "30032E";
            string input2 = "30032";
            string result1 = reverseHex(input1);
            string result2 = reverseHex(input2);
            string expected1 = "2E0330";
            string expected2 = "320003";
            THEN("Hex equals expected hex") {
                REQUIRE(expected1 == result1);
                REQUIRE(expected2 == result2);
            };
        }
    }
}

// conversions
// string ipToHex(string ip);
// void hexToIpv4(string, string *ip);
// void hexToIpv6(string, string *ip);
// string strToUpper(string);
// string hash256(string &payloadString);
// vector<char> hexToBytes(const string &hex);
