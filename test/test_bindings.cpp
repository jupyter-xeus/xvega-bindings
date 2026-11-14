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

    TEST_CASE("process_xvega_input_check")
    {
        xv::df_type df = {
            {"a", {1, 2, 3}},
            {"b", {4.5, 5.5, 6.5}}
        };
        auto tokens = xv_bindings::tokenizer(
            "WIDTH 100 HEIGHT 200 X_FIELD a TYPE ordinal Y_FIELD b AGGREGATE sum "
            "MARK bar COLOR Red GRID false");
        nl::json res = xv_bindings::process_xvega_input(tokens, df);
        const nl::json& spec = res["application/vnd.vegalite.v3+json"];

        CHECK_EQ(spec["width"], 100);
        CHECK_EQ(spec["height"], 200);
        CHECK_EQ(spec["config"]["axis"]["grid"], false);
        CHECK_EQ(spec["encoding"]["x"]["field"], "a");
        CHECK_EQ(spec["encoding"]["x"]["type"], "ordinal");
        CHECK_EQ(spec["encoding"]["y"]["field"], "b");
        CHECK_EQ(spec["encoding"]["y"]["type"], "quantitative");
        CHECK_EQ(spec["encoding"]["y"]["aggregate"], "sum");
        CHECK_EQ(spec["mark"]["type"], "bar");
        CHECK_EQ(spec["mark"]["color"], "red");
        CHECK_EQ(spec["data"]["values"].size(), 3);
    }

    TEST_CASE("process_xvega_input_default_grid")
    {
        xv::df_type df = {{"a", {1, 2}}};
        auto tokens = xv_bindings::tokenizer("X_FIELD a MARK point");
        nl::json res = xv_bindings::process_xvega_input(tokens, df);
        const nl::json& spec = res["application/vnd.vegalite.v3+json"];
        CHECK_EQ(spec["config"]["axis"]["grid"], true);
    }

    TEST_CASE("process_xvega_input_invalid")
    {
        xv::df_type df = {{"a", {1, 2}}};
        auto tokens = xv_bindings::tokenizer("X_FIELD a MARK unknown");
        CHECK_THROWS(xv_bindings::process_xvega_input(tokens, df));
    }
}
