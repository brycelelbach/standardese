// Copyright (C) 2016-2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <standardese/markup/document.hpp>

#include <catch.hpp>

#include <standardese/markup/paragraph.hpp>

using namespace standardese::markup;

template <typename T>
void test_main_sub_document()
{
    auto html = R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Hello World!</title>
</head>
<body>
<p>foo</p>
</body>
</html>
)";

    typename T::builder builder("Hello World!", "my-file");
    builder.add_child(paragraph::builder(block_id("")).add_child(text::build("foo")).finish());

    auto doc = builder.finish();
    REQUIRE(doc->output_name().name() == "my-file");
    REQUIRE(doc->output_name().needs_extension());
    REQUIRE(as_html(*doc) == html);
}

TEST_CASE("main_document", "[markup]")
{
    test_main_sub_document<main_document>();
}

TEST_CASE("sub_document", "[markup]")
{
    test_main_sub_document<sub_document>();
}

TEST_CASE("template_document", "[markup]")
{
    auto html = R"(<section class="standardese-template-document">
<p></p>
</section>
)";

    template_document::builder builder("Hello Templated World!", "foo.bar.baz");
    builder.add_child(paragraph::builder(block_id("")).finish());

    auto doc = builder.finish();
    REQUIRE(doc->output_name().name() == "foo.bar.baz");
    REQUIRE(!doc->output_name().needs_extension());
    REQUIRE(as_html(*doc) == html);
}
