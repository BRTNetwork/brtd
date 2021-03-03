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

#ifndef RIPPLE_PEERFINDER_SIM_PARAMS_H_INCLUDED
#define RIPPLE_PEERFINDER_SIM_PARAMS_H_INCLUDED

namespace ripple {
namespace PeerFinder {
namespace Sim {

/** Defines the parameters for a network simulation. */
struct Params
{
    Params() : steps(50), nodes(10), maxPeers(20), outPeers(9.5), firewalled(0)
    {
    }

    int steps;
    int nodes;
    int maxPeers;
    double outPeers;
    double firewalled;  // [0, 1)
};

}  // namespace Sim
}  // namespace PeerFinder
}  // namespace ripple

#endif
