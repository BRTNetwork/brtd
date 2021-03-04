//------------------------------------------------------------------------------
/*
    This file is part of brtd: https://github.com/BRTNetwork/brtd
    Copyright (c) 2019 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <ripple/beast/unit_test.h>
#include <ripple/overlay/impl/ProtocolVersion.h>

namespace ripple {

class ProtocolVersion_test : public beast::unit_test::suite
{
private:
    template <class FwdIt>
    static std::string
    join(FwdIt first, FwdIt last, char const* sep = ",")
    {
        std::string result;
        if (first == last)
            return result;
        result = to_string(*first++);
        while (first != last)
            result += sep + to_string(*first++);
        return result;
    }

    void
    check(std::string const& s, std::string const& answer)
    {
        auto const result = parseProtocolVersions(s);
        BEAST_EXPECT(join(result.begin(), result.end()) == answer);
    }

public:
    void
    run() override
    {
        testcase("Convert protocol version to string");
        BEAST_EXPECT(to_string(make_protocol(1, 2)) == "RTXP/1.2");
        BEAST_EXPECT(to_string(make_protocol(1, 3)) == "BRTL/1.3");
        BEAST_EXPECT(to_string(make_protocol(2, 0)) == "BRTL/2.0");
        BEAST_EXPECT(to_string(make_protocol(2, 1)) == "BRTL/2.1");
        BEAST_EXPECT(to_string(make_protocol(10, 10)) == "BRTL/10.10");

        {
            testcase("Convert strings to protocol versions");

            // Empty string
            check("", "");
            check(
                "RTXP/1.1,RTXP/1.3,BRTL/2.1,RTXP/1.2,BRTL/2.0",
                "RTXP/1.2,BRTL/2.0,BRTL/2.1");
            check(
                "RTXP/0.9,RTXP/1.01,BRTL/0.3,BRTL/2.01,BRTL/19.04,Oscar/"
                "123,NIKB",
                "");
            check(
                "RTXP/1.2,BRTL/2.0,RTXP/1.2,BRTL/2.0,BRTL/19.4,BRTL/7.89,BRTL/"
                "A.1,BRTL/2.01",
                "RTXP/1.2,BRTL/2.0,BRTL/7.89,BRTL/19.4");
            check(
                "BRTL/2.0,BRTL/3.0,BRTL/4,BRTL/,BRTL,OPT BRTL/2.2,BRTL/5.67",
                "BRTL/2.0,BRTL/3.0,BRTL/5.67");
        }

        {
            testcase("Protocol version negotiation");

            BEAST_EXPECT(
                negotiateProtocolVersion("RTXP/1.2") == make_protocol(1, 2));
            BEAST_EXPECT(
                negotiateProtocolVersion("RTXP/1.2, BRTL/2.0") ==
                make_protocol(2, 0));
            BEAST_EXPECT(
                negotiateProtocolVersion("BRTL/2.0") == make_protocol(2, 0));
            BEAST_EXPECT(
                negotiateProtocolVersion("RTXP/1.2, BRTL/2.0, BRTL/999.999") ==
                make_protocol(2, 0));
            BEAST_EXPECT(
                negotiateProtocolVersion("BRTL/999.999, WebSocket/1.0") ==
                boost::none);
            BEAST_EXPECT(negotiateProtocolVersion("") == boost::none);
        }
    }
};

BEAST_DEFINE_TESTSUITE(ProtocolVersion, overlay, ripple);

}  // namespace ripple
