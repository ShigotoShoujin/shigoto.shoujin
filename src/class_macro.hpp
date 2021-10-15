#pragma once

#define DISABLE_COPY_CONSTRUCTOR(type_name) \
	type_name(const type_name&) = delete;

#define DISABLE_COPY_ASSIGNMENT(type_name) \
	type_name& operator=(const type_name&) = delete;

#define DISABLE_MOVE_CONSTRUCTOR(type_name) \
	type_name(type_name&&) = delete;

#define DISABLE_MOVE_ASSIGNMENT(type_name) \
	type_name& operator=(type_name&&) = delete;

#define DISABLE_COPY(type_name) \
	DISABLE_COPY_CONSTRUCTOR(type_name) \
	DISABLE_COPY_ASSIGNMENT(type_name);

#define DISABLE_MOVE(type_name) \
	DISABLE_MOVE_CONSTRUCTOR(type_name) \
	DISABLE_MOVE_ASSIGNMENT(type_name);

#define DISABLE_COPY_AND_MOVE(type_name) \
	DISABLE_COPY(type_name) \
	DISABLE_MOVE(type_name);
