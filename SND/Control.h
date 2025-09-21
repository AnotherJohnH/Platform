//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Control signal support

#pragma once


#include <cassert>


namespace SND {

//! Base class for ControlIn
class Control
{
public:
   class Observer
   {
   public:
      virtual void controlEvent(Control* control) = 0;
   };

   void setObserver(Observer* observer_) { observer = observer_; }

protected:
   Control(Observer* observer_ = nullptr)
      : observer(observer_)
   {}

   void notifyObserver()
   {
      if(observer != nullptr)
      {
         // Temporarily clear then restore observer to prevent infinite recursion
         Observer* save_observer = observer;
         observer                = nullptr;

         save_observer->controlEvent(this);

         observer = save_observer;
      }
   }

private:
   Observer* observer{nullptr};
};


template <typename TYPE>
class ControlOut;


//! Control signal sink
template <typename TYPE>
class ControlIn : public Control
{
public:
   ControlIn(Control::Observer* observer_ = nullptr)
      : Control(observer_)
   {}

   ControlIn(const TYPE& initial)
      : value(initial)
   {}

   //! The current value of this input
   const TYPE& recv() const { return value; }

   //! Set the value of this input
   void set(const TYPE& value_)
   {
      value = value_;
      notifyObserver();
   }

   //! The current value of this input
   operator const TYPE&() const { return recv(); }

   //! Set the value of this input
   const TYPE& operator=(const TYPE& value_)
   {
      set(value_);
      return value_;
   }

private:
   friend class ControlOut<TYPE>;

   //! Insert this input to a the start of the list
   void link(ControlIn<TYPE>*& list)
   {
      assert(nullptr == next);

      next = list;
      list = this;
   }

   //! Set the value of this input and any downstream inputs
   void deliver(const TYPE& value_)
   {
      operator=(value_);

      if(next)
      {
         next->deliver(value_);
      }
   }

   ControlIn<TYPE>* next{nullptr};
   TYPE             value{};
};


//! Control signal source
template <typename TYPE>
class ControlOut
{
public:
   ControlOut(const TYPE& value_ = TYPE{})
      : value(value_)
   {}

   void operator>>(ControlIn<TYPE>& in)
   {
      in.link(list);

      // in = value;
   }

   //! The current value of this input
   const TYPE& get() const { return value; }

   //! Drive the value of this output
   void send(const TYPE& value_)
   {
      if(value == value_) return;

      value = value_;

      if(list)
      {
         list->deliver(value);
      }
   }

   //! The current value of this output
   operator const TYPE&() const { return get(); }

   //! Drive the value of this output
   void operator=(const TYPE& value_) { send(value_); }

private:
   ControlIn<TYPE>* list{nullptr};
   TYPE             value{};
};


//! Signal interconnect
template <typename TYPE>
class ControlPort : public ControlIn<TYPE>
                  , public ControlOut<TYPE>
                  , public Control::Observer
{
private:
   virtual void controlEvent(Control*) override
   {
      this->send(this->recv());
   }

public:
   ControlPort()
      : ControlIn<TYPE>(this)
   {}

   //! Set the value of this port
   void operator=(const TYPE& value_)
   {
      this->send(value_);
   }
};

} // namespace SND

