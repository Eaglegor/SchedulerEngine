#pragma once

#include <cstddef>
#include <vector>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Stop;
	class Run;

	/** 
		This is a linear vector representation of run's stops including start and end stops which are processed differently from the work stops.
	*/
	class SCENEMANAGER_EXPORT RunStopsView
	{
	public:
		RunStopsView(Run* run);

		class SCENEMANAGER_EXPORT iterator
		{
		public:
			explicit iterator(Run* run);
			explicit iterator(Run* run, size_t current_index);
			iterator(const iterator& rhs);

			Stop* operator*();
			iterator& operator=(const iterator& rhs);
			iterator& operator--();
			iterator& operator++();
			iterator operator--(int);
			iterator operator++(int);
			iterator operator+(size_t offset) const;
			iterator operator-(size_t offset) const;
			bool operator==(const iterator &rhs);
			bool operator!=(const iterator &rhs);

			size_t index() const;

		private:
			Run* run;
			size_t current_index;
		};

		const iterator& begin() const;
		const iterator& end() const;

		Stop* operator[](size_t index);

		size_t size() const;
		bool empty() const;

	private:
		Run* run;

		iterator begin_iterator;
		mutable iterator end_iterator;
	};


	/**
	This is a linear vector representation of run's stops including start and end stops which are processed differently from the work stops.
	*/
	class SCENEMANAGER_EXPORT ConstRunStopsView
	{
	public:
		ConstRunStopsView(const Run* run);

		class SCENEMANAGER_EXPORT iterator
		{
		public:
			explicit iterator(const Run* run);
			explicit iterator(const Run* run, size_t current_index);
			iterator(const iterator& rhs);

			const Stop* operator*();
			iterator& operator=(const iterator& rhs);
			iterator& operator--();
			iterator& operator++();
			iterator operator--(int);
			iterator operator++(int);
			iterator operator+(size_t offset) const;
			iterator operator-(size_t offset) const;
			bool operator==(const iterator &rhs);
			bool operator!=(const iterator &rhs);

			size_t index() const;

		private:
			const Run* run;
			size_t current_index;
		};

		const iterator& begin() const;
		const iterator& end() const;

		const Stop* operator[](size_t index);

		size_t size() const;
		bool empty() const;

	private:
		const Run* run;

		iterator begin_iterator;
		mutable iterator end_iterator;
	};
}