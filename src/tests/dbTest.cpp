#include "../db.cpp"
#include "tests-main.hpp"

TEST_CASE("Database", "[db]") {
    GIVEN("A new database") {
        DB testdb("testdb");
        testdb.open();
        testdb.repair();
        int expectedsize;
        string expectedstring;
        WHEN("The database is initialized") {
            expectedsize = 146;
            testdb.init();
            THEN("The expected size increases") {
                REQUIRE(testdb.size() == expectedsize);
            };
        }

        WHEN("Something unique is put in the database") {
            string i("ipstring");
            IpData ipd1(i, "");
            testdb.put("hello", "there");
            testdb.put(ipd1);
            THEN("The expected size increases") {
                expectedsize = 2;
                REQUIRE(testdb.size() == expectedsize);
            }
            WHEN("something is retrieved from the database") {
                string there = testdb.get("hello");
                string ipdStr = testdb.get(i);
                IpData ipd2(i, ipdStr);
                THEN("the retrieved value equals the put value") {
                    expectedstring = "there";
                    REQUIRE(there == expectedstring);
                    REQUIRE(ipd2.getIp() == i);
                }
            }
        }
        WHEN("Something non-unique is put in the database") {
            string ip = "ipstring";
            IpData ipd1(ip, "");
            testdb.put("hello", "there");
            testdb.put("hello", "this");
            testdb.put(ipd1);
            testdb.put(ipd1);
            THEN("The expected size increases limited") {
                expectedsize = 2;
                REQUIRE(testdb.size() == expectedsize);
            }
            WHEN("something is retrieved from the database") {
                string ip = "ipstring";
                string there = testdb.get("hello");
                string ipdStr = testdb.get(ip);
                IpData ipd2(ip, ipdStr);
                THEN("the retrieved value equals the put value") {
                    expectedstring = "this";
                    REQUIRE(there == expectedstring);
                    REQUIRE(ipd2.getIp() == ip);
                }
            }
        }
    }
}
TEST_CASE("Database 2", "[db]") {
    GIVEN("A new database") {
        DB testdb("testdb");
        testdb.open();
        testdb.repair();
        int expectedsize;
        string expectedstring;

        WHEN("The database is refreshed") {  // TODO Adds one?
            testdb.put("hello", "there");
            testdb.refresh();
            THEN("The size stays the same") {
                expectedsize = 1;
                REQUIRE(testdb.size() == expectedsize);
            }
        }
        WHEN("An existing entry is deleted") {
            testdb.put("hello", "there");
            testdb.deleteKey("hello");
            THEN("retrieval results in an empty string") {
                expectedstring = "";
                string there = testdb.get("hello");
                REQUIRE(there == expectedstring);
            }
        }
        WHEN("A non-existing entry is deleted") {
            testdb.put("hello", "there");
            testdb.deleteKey("hel");
            THEN("size remains the same") {
                expectedsize = 1;
                REQUIRE(testdb.size() == expectedsize);
            }
        }
        WHEN("the database is read to a vector") {
            vector<IpData> ipdvec = testdb.readToVec();
            THEN("the vector size equals the database size") {
                REQUIRE(ipdvec.size() == testdb.size());
            }
        }
    }
}
