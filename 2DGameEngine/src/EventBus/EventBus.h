#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include <map>
#include <functional>
#include <typeindex>
#include <list>

/**
 * 
 */
class Event
{
public:
	Event() = default;
	virtual ~Event() = default;
};

/**
 * 
 */
class IEventCallback
{
private:
	virtual void Call(Event& event) = 0;
public:
	virtual ~IEventCallback() = default;

	void Execute(Event& event)
	{
		Call(event);
	}
};

/**
 * 
 */
template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
	typedef void (TOwner::*CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& event) override
	{
		// обезопасить неопределенное поведение если событие не соответсвует ожидаемому
		/**
		 *if (TEvent* specificEvent = dynamic_cast<TEvent*>(&event))
		 *{
         *		std::invoke(callbackFunction, ownerInstance, *specificEvent);
    	 *}
		 */
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent*>(event));
	}

public:
	EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
	{
		this->ownerInstance = ownerInstance;
		this->callbackFunction = callbackFunction;
	}

	virtual ~EventCallback() = default;
};

/**
 * 
 */
using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

/**
 * 
 */
class EventBus
{
	std::map<std::type_index,std::unique_ptr<HandlerList>> subscribers;

public:
	EventBus()
	{
		Logger::Log("Event bus contructor called");
	}

	~EventBus()
	{
		Logger::Log("Event bus destructor called");
	}

	template<typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
	{
		if (!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
		auto subscriber = std::make_unique<EventCallback<TOwner,TEvent>>(ownerInstance, callbackFunction);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	template<typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();
		if (handles)
		{
			for (auto it = handlers->begin(); it != handlers->end(); it++)
			{
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->Execute(event);	
			}
		}
	}
};

#endif
