#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jerryscript.h"

typedef struct
{
	char* data_p;
	unsigned int length;
} buffer_native_object_t;

typedef struct
{
	int area;
	int perimeter;
} shape_native_object_t;

#define SECRET_INFO ((void *) 42)

static void
buffer_native_freecb(void* native_p,
					 jerry_object_native_info_t* info_p)
{
	(void)info_p;

	char* data_p = ((buffer_native_object_t*)native_p)->data_p;

	if (data_p != NULL)
	{
		free(data_p);
	}

	free(native_p);
}

static void
shape_native_freecb(void* native_p,
					jerry_object_native_info_t* info_p)
{
	(void)info_p;

	free(native_p);
}

static void
destructor_freecb(void* native_p,
				  jerry_object_native_info_t* info_p)
{
	(void)native_p;
	(void)info_p;

	printf("Note: the object has been freed\n");
}

// NOTE: The address (!) of type_info acts as a way to uniquely "identify" the
// C type `buffer_native_object_t *`.
static const jerry_object_native_info_t buffer_obj_type_info =
{
  .free_cb = buffer_native_freecb
};

// NOTE: The address (!) of type_info acts as a way to uniquely "identify" the
// C type `shape_native_object_t *`.
static const jerry_object_native_info_t shape_obj_type_info =
{
  .free_cb = shape_native_freecb
};

// NOTE: The address (!) of type_info is the unique "identifier"
static const jerry_object_native_info_t destructor_obj_type_info =
{
  .free_cb = destructor_freecb
};

static void
print_buffer(char* data_p,
			 unsigned int length)
{
	for (unsigned int i = 0; i < length; ++i)
	{
		printf("%c", data_p[i]);
	}

	printf("\n");
}

static void
do_stuff(jerry_value_t object)
{
	void* native_p;
	bool has_p = jerry_get_object_native_pointer(object, &native_p, &buffer_obj_type_info);

	if (!has_p)
	{
		// Process the error
		return;
	}

	// It is safe to cast to buffer_native_object_t * and dereference the pointer:
	buffer_native_object_t* buffer_p = (buffer_native_object_t*)native_p;
	print_buffer(buffer_p->data_p, buffer_p->length); // Usage of buffer_p

	bool need_shape_info = true; // implementation dependent

	if (need_shape_info)
	{
		has_p = jerry_get_object_native_pointer(object, &native_p, &shape_obj_type_info);

		if (!has_p)
		{
			// Process the error
			return;
		}

		// It is safe to cast to shape_native_object_t * and dereference the pointer:
		shape_native_object_t* shape_p = (shape_native_object_t*)native_p;

		printf("Area: %d\tPerimeter: %d\n", shape_p->area, shape_p->perimeter); // Usage of shape_p
	}

	bool need_secret_info = true; // implementation dependent

	if (need_secret_info)
	{
		has_p = jerry_get_object_native_pointer(object, &native_p, NULL);

		if (!has_p)
		{
			// Process the error
			return;
		}

		printf("Secret: %d\n", (int)((uintptr_t)native_p)); // Usage of native_p

		bool deleted = jerry_delete_object_native_pointer(object, NULL);

		if (deleted)
		{
			printf("The secret is no longer available\n");
		}
	}
}

int
main(void)
{
	jerry_init(JERRY_INIT_EMPTY);

	jerry_value_t object = jerry_create_object();
	buffer_native_object_t* buffer_p = (buffer_native_object_t*)malloc(sizeof(buffer_native_object_t));
	buffer_p->length = 14;
	buffer_p->data_p = (char*)malloc(buffer_p->length * sizeof(char));
	memcpy(buffer_p->data_p, "My buffer data", buffer_p->length);
	jerry_set_object_native_pointer(object, buffer_p, &buffer_obj_type_info);

	shape_native_object_t* shape_p = (shape_native_object_t*)malloc(sizeof(shape_native_object_t));
	shape_p->area = 6;
	shape_p->perimeter = 12;
	jerry_set_object_native_pointer(object, shape_p, &shape_obj_type_info);

	// The native pointer can be NULL. This gives possibly to get notified via the native type info's
	// free callback when the object has been freed by the GC.
	jerry_set_object_native_pointer(object, NULL, &destructor_obj_type_info);

	// The native type info can be NULL as well. In this case the registered property is simply freed
	// when the object is freed by the GC.
	jerry_set_object_native_pointer(object, SECRET_INFO, NULL);

	do_stuff(object);

	jerry_release_value(object);
	jerry_cleanup();

	return 0;
}