#include "../ipdata.cpp"
#include "tests-main.hpp"

TEST_CASE("IpData", "[ipdb]") {
    GIVEN("An empty IpData object") {
        IpData ipd("ip", "lon", "lat", "stat", "vers");
        int expectedsize;
        string expectedstring;
        WHEN("The ip is retrieved") {
            expectedstring = "ip";
            string ip = ipd.getIp();
            THEN("The retrieved equals expected ip") {
                REQUIRE(ip == expectedstring);
            };
        }
        WHEN("The lat is retrieved") {
            expectedstring = "lat";
            string lat = ipd.getLat();
            THEN("The retrieved equals expected lat") {
                REQUIRE(lat == expectedstring);
            };
        }
        WHEN("The lon is retrieved") {
            expectedstring = "lon";
            string lon = ipd.getLon();
            THEN("The retrieved equals expected lon") {
                REQUIRE(lon == expectedstring);
            };
        }
        WHEN("The status is retrieved") {
            expectedstring = "stat";
            string status = ipd.getStatus();
            THEN("The retrieved equals expected status") {
                REQUIRE(status == expectedstring);
            };
        }
        WHEN("The size is retrieved") {
            expectedsize = 16;
            int size = ipd.getSize();
            THEN("The retrieved equals expected size") {
                REQUIRE(size == expectedsize);
            };
        }
        WHEN("The lat is set") {
            expectedstring = "latje";
            ipd.setLat("latje");
            THEN("The set equals expected lat") {
                REQUIRE(ipd.getLat() == expectedstring);
            };
        }
        WHEN("The lon is set") {
            expectedstring = "lontje";
            ipd.setLon("lontje");
            THEN("The set equals expected lon") {
                REQUIRE(ipd.getLon() == expectedstring);
            };
        }
        WHEN("The status is set") {
            expectedstring = "statusje";
            ipd.setStatus("statusje");
            THEN("The set equals expected status") {
                REQUIRE(ipd.getStatus() == expectedstring);
            };
        }
        WHEN("The content is serialized") {
            string serial = ipd.serializeAsStr();
            THEN("A new object can be created") {
                IpData ipd2(serial);
                REQUIRE(ipd2.getIp() == ipd.getIp());
                REQUIRE(ipd2.getLat() == ipd.getLat() + "00000");
                REQUIRE(ipd2.getLon() == ipd.getLon() + "000000");
                REQUIRE(ipd2.getStatus() == ipd.getStatus());
            };
        }
    }
}
