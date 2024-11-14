#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include <map>
#include <typeindex>
#include <list>
#include <functional>

class Event
{
public:
	Event() = default;
};


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

template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
	typedef void (TOwner::*CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& event) override
	{
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


using HandlerList = std::list<std::unique_ptr<IEventCallback>> ;

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
		
	}

	template<typename TEvent, typename TOwner>
	void EmitEvent()
	{
			
	}


};



#endif
