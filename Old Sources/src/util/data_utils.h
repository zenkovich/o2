#pragma once

#include "public_namespace.h"

OPEN_O2_NAMESPACE

#define VECTOR_CONTAINS(vectype, vec, pred) { \
for (vectype::iterator it = vec.begin(); it != vec.end(); ++it) \
if (pred) return true; \
	return false; \
}

#define VECTOR_CONTAINS_CONST(vectype, vec, pred) { \
for (vectype::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) \
if (pred) return true; \
	return false; \
}

#define FIND_PRED(vectype, vec, pred) { \
for (vectype::iterator it = vec.begin(); it != vec.end(); ++it) \
if (pred) return it; \
	return vec[0]; \
}

#define FIND(_vector, _search_object) \
	std::find((_vector).begin(), (_vector).end(), (_search_object))

#define FINDS_CONST(_vector, _search_object) \
	std::find((_vector).cbegin(), (_vector).cend(), (_search_object))

#define FOREACH(_vector_type, _vector, _iterator) \
for (_vector_type::iterator _iterator = (_vector).begin(); _iterator != (_vector).end(); ++_iterator)

#define FOREACH_CONST(_vector_type, _vector, _iterator) \
for (_vector_type::const_iterator _iterator = (_vector).cbegin(); _iterator != (_vector).cend(); ++_iterator)

#define FOREACH_BACK(_vector_type, _vector, _iterator) \
for (_vector_type::reverse_iterator _iterator = (_vector).rbegin(); _iterator != (_vector).rend(); ++_iterator)

#define FOREACH_BACK_CONST(_vector_type, _vector, _iterator) \
for (_vector_type::const_reverse_iterator _iterator = (_vector).crbegin(); _iterator != (_vector).crend(); ++_iterator)

#define REPEAT(_count) \
for (int i = 0; i < _count; i++)

#define RELEASE_VECTOR(_vector_type, _vector) \
for (_vector_type::iterator _iterator = (_vector).begin(); _iterator != (_vector).end(); ++_iterator) \
	safe_release(*_iterator); \
	(_vector).clear()

CLOSE_O2_NAMESPACE
