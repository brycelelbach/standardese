// Copyright (C) 2016-2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef STANDARDESE_MARKUP_DOCUMENT_HPP_INCLUDED
#define STANDARDESE_MARKUP_DOCUMENT_HPP_INCLUDED

#include <utility>

#include <type_safe/optional_ref.hpp>

#include <standardese/markup/block.hpp>
#include <standardese/markup/entity.hpp>

namespace standardese
{
    namespace markup
    {
        /// Base class for entities representing a stand-alone document.
        ///
        /// Those are the root nodes of the markup AST.
        class document_entity : public entity, public entity_container<block_entity>
        {
        public:
            /// \returns The title of the document.
            const std::string& title() const noexcept
            {
                return title_;
            }

            /// \returns The output name of the document.
            const markup::output_name& output_name() const noexcept
            {
                return output_name_;
            }

        protected:
            document_entity(std::string title, markup::output_name name)
            : output_name_(std::move(name)), title_(std::move(title))
            {
            }

        private:
            markup::output_name output_name_;
            std::string         title_;
        };

        /// A document which is the main output page of the documentation.
        class main_document final : public document_entity
        {
        public:
            /// Builds the main document.
            class builder : public container_builder<main_document>
            {
            public:
                /// \effects Creates an empty document with given title and output file name.
                builder(std::string title, std::string output_name)
                : container_builder(std::unique_ptr<main_document>(
                      new main_document(std::move(title), std::move(output_name))))
                {
                }
            };

        private:
            void do_append_html(std::string& result) const override;

            main_document(std::string title, std::string name)
            : document_entity(std::move(title), markup::output_name::from_name(std::move(name)))
            {
            }
        };

        void generate_html(const main_document& document);

        /// A document which is not the main output page of the documentation.
        class sub_document final : public document_entity
        {
        public:
            /// Builds the sub document.
            class builder : public container_builder<sub_document>
            {
            public:
                /// \effects Creates an empty document with given title and output file name.
                builder(std::string title, std::string output_name)
                : container_builder(std::unique_ptr<sub_document>(
                      new sub_document(std::move(title), std::move(output_name))))
                {
                }
            };

        private:
            void do_append_html(std::string& result) const override;

            sub_document(std::string title, std::string name)
            : document_entity(std::move(title), markup::output_name::from_name(std::move(name)))
            {
            }
        };

        void generate_html(const sub_document& document);

        class template_document final : public document_entity
        {
        public:
            /// Builds a template document.
            class builder : public container_builder<template_document>
            {
            public:
                /// \effects Creates it given the title and the file name of the template file.
                /// If the file name does not contain an extension, the document's [standardese::markup::output_name]() will need one.
                builder(std::string title, std::string file_name)
                : container_builder(std::unique_ptr<template_document>(
                      new template_document(std::move(title), std::move(file_name))))
                {
                }
            };

        private:
            void do_append_html(std::string& result) const override;

            template_document(std::string title, std::string file_name);
        };
    }
} // namespace standardese::markup

#endif // STANDARDESE_MARKUP_DOCUMENT_HPP_INCLUDED
