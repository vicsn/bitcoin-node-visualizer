#include "../ipdata.cpp"
#include "tests-main.hpp"

TEST_CASE("IpData", "[ipdb]") {
    GIVEN("An empty IpData object") {
        IpData ipdb("ip", "lat", "lon", "stat");
        int expectedsize;
        string expectedstring;
        WHEN("The ip is retrieved") {
            expectedstring = "ip";
            string ip = ipdb.getIp();
            THEN("The retrieved equals expected ip") {
                REQUIRE(ip == expectedstring);
            };
        }
        WHEN("The lat is retrieved") {
            expectedstring = "lat";
            string lat = ipdb.getLat();
            THEN("The retrieved equals expected lat") {
                REQUIRE(lat == expectedstring);
            };
        }
        WHEN("The lon is retrieved") {
            expectedstring = "lon";
            string lon = ipdb.getLon();
            THEN("The retrieved equals expected lon") {
                REQUIRE(lon == expectedstring);
            };
        }
        WHEN("The status is retrieved") {
            expectedstring = "stat";
            string status = ipdb.getStatus();
            THEN("The retrieved equals expected status") {
                REQUIRE(status == expectedstring);
            };
        }
        WHEN("The size is retrieved") {
            expectedsize = 12;
            int size = ipdb.getSize();
            THEN("The retrieved equals expected size") {
                REQUIRE(size == expectedsize);
            };
        }
        WHEN("The lat is set") {
            expectedstring = "latje";
            ipdb.setLat("latje");
            THEN("The set equals expected lat") {
                REQUIRE(ipdb.getLat() == expectedstring);
            };
        }
        WHEN("The lon is set") {
            expectedstring = "lontje";
            ipdb.setLon("lontje");
            THEN("The set equals expected lon") {
                REQUIRE(ipdb.getLon() == expectedstring);
            };
        }
        WHEN("The status is set") {
            expectedstring = "statusje";
            ipdb.setStatus("statusje");
            THEN("The set equals expected status") {
                REQUIRE(ipdb.getStatus() == expectedstring);
            };
        }
        WHEN("The content is serialized") {
            string serial = ipdb.serializeAsStr();
            THEN("A new object can be created") {
                IpData ipdb2(serial);
                REQUIRE(ipdb2.getIp() == ipdb.getIp());
                REQUIRE(ipdb2.getLat() == ipdb.getLat() + "000000");
                REQUIRE(ipdb2.getLon() == ipdb.getLon() + "000000");
                REQUIRE(ipdb2.getStatus() == ipdb.getStatus());
            };
        }
    }
}
