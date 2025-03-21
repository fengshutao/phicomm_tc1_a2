/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_TICKER_H
#define MBED_TICKER_H

#include "TimerEvent.h"
#include "Callback.h"

namespace mbed {

/** A Ticker is used to call a function at a recurring interval
 *
 *  You can use as many seperate Ticker objects as you require.
 *
 * @Note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Toggle the blinking led after 5 seconds
 *
 * #include "mbed.h"
 *
 * Ticker timer;
 * DigitalOut led1(LED1);
 * DigitalOut led2(LED2);
 *
 * int flip = 0;
 *
 * void attime() {
 *     flip = !flip;
 * }
 *
 * int main() {
 *     timer.attach(&attime, 5);
 *     while(1) {
 *         if(flip == 0) {
 *             led1 = !led1;
 *         } else {
 *             led2 = !led2;
 *         }
 *         wait(0.2);
 *     }
 * }
 * @endcode
 */
class Ticker : public TimerEvent {

public:
    Ticker() : TimerEvent() {
#ifdef _HSF_
		_argument = NULL;
		_callback = NULL;
#endif
    }

    Ticker(const ticker_data_t *data) : TimerEvent(data) {
        data->interface->init();
    }

    /** Attach a function to be called by the Ticker, specifiying the interval in seconds
     *
     *  @param func pointer to the function to be called
     *  @param t the time between calls in seconds
     */
    void attach(Callback<void()> func, float t) {
        attach_us(func, t * 1000000.0f);
    }

#ifdef _HSF_
	void attach_us(void (*func)(void const*) ,void *arg,timestamp_t t) {
		   setup(t);
		   _argument = arg;
		   _callback = func;
	   }
#endif
    /** Attach a member function to be called by the Ticker, specifiying the interval in seconds
     *
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to be called
     *  @param t the time between calls in seconds
     */
    template<typename T, typename M>
    void attach(T *obj, M method, float t) {
        attach(Callback<void()>(obj, method), t);
    }

    /** Attach a function to be called by the Ticker, specifiying the interval in micro-seconds
     *
     *  @param fptr pointer to the function to be called
     *  @param t the time between calls in micro-seconds
     */
    void attach_us(Callback<void()> func, timestamp_t t) {
        _function.attach(func);
        setup(t);
    }

    /** Attach a member function to be called by the Ticker, specifiying the interval in micro-seconds
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param t the time between calls in micro-seconds
     */
    template<typename T, typename M>
    void attach_us(T *obj, M method, timestamp_t t) {
        attach_us(Callback<void()>(obj, method), t);
    }

    virtual ~Ticker() {
        detach();
    }

    /** Detach the function
     */
    void detach();

protected:
    void setup(timestamp_t t);
    virtual void handler();

protected:
    timestamp_t         _delay;     /**< Time delay (in microseconds) for re-setting the multi-shot callback. */
    Callback<void()>    _function;  /**< Callback. */
#ifdef _HSF_
	void                *_argument;
	void (*_callback)(void const*)  ;
#endif
};

} // namespace mbed

#endif
