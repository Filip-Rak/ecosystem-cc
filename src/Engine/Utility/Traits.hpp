#pragma once

struct NonCopyable
{
	NonCopyable()  = default;
	~NonCopyable() = default;

	NonCopyable( const NonCopyable& )                   = delete;
	auto operator=( const NonCopyable& ) -> NonCopyable = delete;
};

struct NonMoveable
{
	NonMoveable()  = default;
	~NonMoveable() = default;

	NonMoveable( NonMoveable&& )                    = delete;
	auto operator=( NonMoveable&& ) -> NonMoveable& = delete;
};