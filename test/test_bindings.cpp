/***************************************************************************
* Copyright (c) 2024, QuantStack and xeus-SQLite contributors              *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "doctest/doctest.h"

#include "xvega-bindings/xvega_bindings.hpp"

TEST_SUITE("xvega-bindings")
{
    TEST_CASE("sanitize_string_check")
    {
        std::string code = "\n\n Some inp\nut\n";
        std::string sanitized_string;
        sanitized_string = xv_bindings::sanitize_string(code);
        CHECK_EQ(sanitized_string, " Some input");
    }

    TEST_CASE("tokenizer_check")
    {
        std::string code = "%LOAD  database.db   rw";
        std::vector<std::string> tokenized_code;
        tokenized_code = xv_bindings::tokenizer(code);
        CHECK_EQ(tokenized_code[1], "database.db");
    }
}
