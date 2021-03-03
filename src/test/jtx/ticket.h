//------------------------------------------------------------------------------
/*
    This file is part of brtd: https://github.com/ripple/brtd
    Copyright (c) 2012, 2013 Ripple Labs Inc.

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

#ifndef RIPPLE_TEST_JTX_TICKET_H_INCLUDED
#define RIPPLE_TEST_JTX_TICKET_H_INCLUDED

#include <boost/optional.hpp>
#include <cstdint>
#include <test/jtx/Account.h>
#include <test/jtx/Env.h>
#include <test/jtx/owners.h>

namespace ripple {
namespace test {
namespace jtx {

/*
    This shows how the jtx system may be extended to other
    generators, funclets, conditions, and operations,
    without changing the base declarations.
*/

/** Ticket operations */
namespace ticket {

/** Create one of more tickets */
Json::Value
create(Account const& account, std::uint32_t count);

/** Set a ticket sequence on a JTx. */
class use
{
private:
    std::uint32_t ticketSeq_;

public:
    use(std::uint32_t ticketSeq) : ticketSeq_{ticketSeq}
    {
    }

    void
    operator()(Env&, JTx& jt) const;
};

}  // namespace ticket

/** Match the number of tickets on the account. */
using tickets = owner_count<ltTICKET>;

}  // namespace jtx

}  // namespace test
}  // namespace ripple

#endif
