#ifndef __CHAR__UTILITIES_HPP__
#define __CHAR__UTILITIES_HPP__

#include <char.hpp>

namespace test_utils
{
class CharacterBuilder
{
	public:
		using result_t = CHAR_DATA::shared_ptr;

		void create_new();
		void create_character_with_one_removable_affect();
		void create_character_with_two_removable_affects();
		void create_character_with_two_removable_and_two_not_removable_affects();

		void add_poison();
		void add_sleep();
		void add_detect_invis();
		void add_detect_align();

		result_t get() const { return m_result; }

	private:
		result_t m_result;
};
}

#endif // __CHAR__UTILITIES_HPP__

// vim: ts=4 sw=4 tw=0 noet syntax=cpp :
