//------------------------------------------------------------------------------
/*
    This file is part of brtd: https://github.com/BRTNetwork/brtd
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

#ifndef RIPPLE_PROTOCOL_TOKENS_H_INCLUDED
#define RIPPLE_PROTOCOL_TOKENS_H_INCLUDED

#include <boost/optional.hpp>
#include <cstdint>
#include <string>

namespace ripple {

enum class TokenType : std::uint8_t {
    None = 1,  // unused
    NodePublic = 28,
    NodePrivate = 32,
    AccountID = 0,
    AccountPublic = 35,
    AccountSecret = 34,
    FamilyGenerator = 41,  // unused
    FamilySeed = 33
};

template <class T>
boost::optional<T>
parseBase58(std::string const& s);

template <class T>
boost::optional<T>
parseBase58(TokenType type, std::string const& s);

/** Encode data in Base58Check format using XRPL alphabet

    For details on the format see
    https://xrpl.org/base58-encodings.html#base58-encodings

    @param type The type of token to encode.
    @param token Pointer to the data to encode.
    @param size The size of the data to encode.

    @return the encoded token.
*/
std::string
encodeBase58Token(TokenType type, void const* token, std::size_t size);

/** Decode a token of given type encoded using Base58Check and the XRPL alphabet

    @param s The encoded token
    @param type The type expected for this token.

    @return If the encoded token decodes correctly, the token data without
    the type or checksum. And empty string otherwise.
*/
std::string
decodeBase58Token(std::string const& s, TokenType type);

}  // namespace ripple

#endif
