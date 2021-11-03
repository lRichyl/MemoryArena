#include <stdio.h>
#include <stdint.h>

#include "memory_arena.h"
#include "time.h"

// This is a test for a basic memory arena allocator capable of allocating any type.
// Currently fragmentation is not being handled but it is still functional.
// Author: Ricardo Trejo Castro
// Date: November 3rd 2021

// Running on a Ryzen 3 3200G
// Time ranges for allocating 100000 ints.
// Arena           new
// 280us - 370us   5.8ms - 6.1ms
// Maximum speed-up factor : 20x.
// Minimun speed-up factor : 16x.

//Compile with g++ using -O2.


struct Person{
	char *name;
	int age;
};

int main(void){
	{
		MemoryArena arena;
		init_memory_arena(&arena, 1024); // The arena is initialized with a given size in bytes.
		
		int *a = NULL;
		float *b = NULL;
		Person *p = NULL;
		
		// The template function "allocate_from_arena" is used to allocate any type in the arena.
		a = allocate_from_arena<int>(&arena);
		b = allocate_from_arena<float>(&arena);
		p = allocate_from_arena<Person>(&arena);
		printf("%d\n", sizeof(*a));
		printf("%d\n", sizeof(*b));
		printf("%d\n", sizeof(*p));
		assert(a);
		*a = 25;
		*b = 3.5;
		
		p->name = "Richy";
		p->age = 24;
		// void *r = new char[1000]  {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
		
		// int *a = (int*)(r);
		
		printf("%d\n", *a);
		printf("%f\n", *b);
		printf("%s\n",p->name);
		printf("%d\n",p->age);
		// printf("Person : %X\n", p);
		
		// The arena info is printed to see how much memory we have left before and after calling the function "free_from_arena".
		print_arena_info(&arena);
		free_from_arena(&arena, a);
		print_arena_info(&arena);
	}
	
	{
		//This purpose of this part is to test the functionality of "free_from_arena".
		printf("\nSEGUNDA PARTE\n");
		MemoryArena arena;
		init_memory_arena(&arena, 12);
		int *a;
		int *b;
		int *c;
		int *d;
		
		a = allocate_from_arena<int>(&arena);
		b = allocate_from_arena<int>(&arena);
		c = allocate_from_arena<int>(&arena);
		*a = 101;
		*b = 250;
		*c = 23;
		printf("%d\n", *a);
		printf("%d\n", *b);
		printf("%d\n", *c);
		
		//If the following line is commented the allocation will fail and the program will end because there is no more room in the arena.
		free_from_arena(&arena, b);
		
		d = allocate_from_arena<int>(&arena);
		*d = 45;
		printf("%d\n", *d);
		
		print_arena_info(&arena);
	}
	
	long long perf_count_frequency = get_performance_counter_frequency();
	const int NUMBER_OF_INTS = 100000;
	{
		printf("\nSPEED TEST : INT\n");
		MemoryArena arena;
		init_memory_arena(&arena, NUMBER_OF_INTS * sizeof(int));
		LARGE_INTEGER start_time = get_time_counter();
		
		int *ptr;
		for(int i = 0; i < NUMBER_OF_INTS; i++){
			ptr = allocate_from_arena<int>(&arena);
		}
		
		LARGE_INTEGER end_time = get_time_counter();
		float ms = get_time_in_ms(start_time, end_time, perf_count_frequency);
		printf("Allocation time using arena: %f\n", ms/1000.0f);
		
	}
	
	{
		LARGE_INTEGER start_time = get_time_counter();	
		for(int i = 0; i < NUMBER_OF_INTS; i++){
			int *ptr = new int;
		}
		
		LARGE_INTEGER end_time = get_time_counter();
		float ms = get_time_in_ms(start_time, end_time, perf_count_frequency);
		printf("Allocation time using new: %f\n", ms/1000.0f);
	}
	
	
	const int NUMBER_OF_PERSONS = 100000;
	{
		printf("\nSPEED TEST : STRUCTS\n");
		MemoryArena arena;
		init_memory_arena(&arena, NUMBER_OF_PERSONS * sizeof(Person));
		LARGE_INTEGER start_time = get_time_counter();
		
		
		for(int i = 0; i < NUMBER_OF_PERSONS; i++){
			Person *person = allocate_from_arena<Person>(&arena);
		}
		
		LARGE_INTEGER end_time = get_time_counter();
		float ms = get_time_in_ms(start_time, end_time, perf_count_frequency);
		printf("Allocation time using arena: %f\n", ms/1000.0f);
		
	}
	
	{
		LARGE_INTEGER start_time = get_time_counter();	
		for(int i = 0; i < NUMBER_OF_PERSONS; i++){
			Person *person = new Person;
		}
		
		LARGE_INTEGER end_time = get_time_counter();
		float ms = get_time_in_ms(start_time, end_time, perf_count_frequency);
		printf("Allocation time using new: %f\n", ms/1000.0f);
	}
	
		
	return 0;
}