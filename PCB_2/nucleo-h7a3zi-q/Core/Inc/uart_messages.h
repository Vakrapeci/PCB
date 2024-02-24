/*
 *  Copyright (C) 2020-2022 Embedded AMS B.V. - All Rights Reserved
 *
 *  This file is part of Embedded Proto.
 *
 *  Embedded Proto is open source software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, version 3 of the license.
 *
 *  Embedded Proto  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
 *
 *  For commercial and closed source application please visit:
 *  <https://EmbeddedProto.com/license/>.
 *
 *  Embedded AMS B.V.
 *  Info:
 *    info at EmbeddedProto dot com
 *
 *  Postal address:
 *    Johan Huizingalaan 763a
 *    1066 VH, Amsterdam
 *    the Netherlands
 */

// This file is generated. Please do not edit!
#ifndef UART_MESSAGES_H
#define UART_MESSAGES_H

#include <cstdint>
#include <MessageInterface.h>
#include <WireFormatter.h>
#include <Fields.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedFieldFixedSize.h>
#include <FieldStringBytes.h>
#include <Errors.h>
#include <limits>

// Include external proto definitions


class Command final: public ::EmbeddedProto::MessageInterface
{
  public:
    Command() = default;
    Command(const Command& rhs )
    {
      set_button(rhs.get_button());
      set_value(rhs.get_value());
    }

    Command(const Command&& rhs ) noexcept
    {
      set_button(rhs.get_button());
      set_value(rhs.get_value());
    }

    ~Command() override = default;

    enum class Buttons : uint32_t
    {
      DoNothing = 0,
      On = 1,
      Off = 2,
      Stop = 3
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      BUTTON = 1,
      VALUE = 2
    };

    Command& operator=(const Command& rhs)
    {
      set_button(rhs.get_button());
      set_value(rhs.get_value());
      return *this;
    }

    Command& operator=(const Command&& rhs) noexcept
    {
      set_button(rhs.get_button());
      set_value(rhs.get_value());
      return *this;
    }

    static constexpr char const* BUTTON_NAME = "button";
    inline void clear_button() { button_.clear(); }
    inline void set_button(const Buttons& value) { button_ = value; }
    inline void set_button(const Buttons&& value) { button_ = value; }
    inline const Buttons& get_button() const { return button_.get(); }
    inline Buttons button() const { return button_.get(); }

    static constexpr char const* VALUE_NAME = "value";
    inline void clear_value() { value_.clear(); }
    inline void set_value(const uint32_t& value) { value_ = value; }
    inline void set_value(const uint32_t&& value) { value_ = value; }
    inline uint32_t& mutable_value() { return value_.get(); }
    inline const uint32_t& get_value() const { return value_.get(); }
    inline uint32_t value() const { return value_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Buttons>(0) != button_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = button_.serialize_with_id(static_cast<uint32_t>(FieldNumber::BUTTON), buffer, false);
      }

      if((0U != value_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = value_.serialize_with_id(static_cast<uint32_t>(FieldNumber::VALUE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::BUTTON:
            return_value = button_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::VALUE:
            return_value = value_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_button();
      clear_value();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::BUTTON:
          name = BUTTON_NAME;
          break;
        case FieldNumber::VALUE:
          name = VALUE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

    private:


      EmbeddedProto::enumeration<Buttons> button_ = static_cast<Buttons>(0);
      EmbeddedProto::uint32 value_ = 0U;

};

class Reply final: public ::EmbeddedProto::MessageInterface
{
  public:
    Reply() = default;
    Reply(const Reply& rhs )
    {
      set_LED_state(rhs.get_LED_state());
    }

    Reply(const Reply&& rhs ) noexcept
    {
      set_LED_state(rhs.get_LED_state());
    }

    ~Reply() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      LED_STATE = 1
    };

    Reply& operator=(const Reply& rhs)
    {
      set_LED_state(rhs.get_LED_state());
      return *this;
    }

    Reply& operator=(const Reply&& rhs) noexcept
    {
      set_LED_state(rhs.get_LED_state());
      return *this;
    }

    static constexpr char const* LED_STATE_NAME = "LED_state";
    inline void clear_LED_state() { LED_state_.clear(); }
    inline void set_LED_state(const bool& value) { LED_state_ = value; }
    inline void set_LED_state(const bool&& value) { LED_state_ = value; }
    inline bool& mutable_LED_state() { return LED_state_.get(); }
    inline const bool& get_LED_state() const { return LED_state_.get(); }
    inline bool LED_state() const { return LED_state_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((false != LED_state_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = LED_state_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LED_STATE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::LED_STATE:
            return_value = LED_state_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_LED_state();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::LED_STATE:
          name = LED_STATE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

    private:


      EmbeddedProto::boolean LED_state_ = false;

};

#endif // UART_MESSAGES_H