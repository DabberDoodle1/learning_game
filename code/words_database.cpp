#include "words_database.hpp"
#include <iostream>

std::vector<WordData> WordDatabase::words;
std::map<const std::string, std::vector<const WordData*>> WordDatabase::word_groups;

void WordDatabase::init()
{
    // Populating word database
    // Pronouns section
    words.emplace_back("나", "I (informal)");
    words.emplace_back("저", "I (polite)");
    words.emplace_back("너", "You (informal)");
    words.emplace_back("당신", "You (formal)");
    words.emplace_back("우리", "We, our");
    words.emplace_back("그", "He");
    words.emplace_back("그녀", "She");
    words.emplace_back("이것", "This thing");
    words.emplace_back("그것", "That thing");

    // Nouns section
    words.emplace_back("사람", "Person");
    words.emplace_back("친구", "Friend");
    words.emplace_back("가족", "Family");
    words.emplace_back("엄마", "Mom");
    words.emplace_back("아빠", "Dad");
    words.emplace_back("아이", "Child");
    words.emplace_back("집", "House, Home");

    words.emplace_back("학교", "School");
    words.emplace_back("회사", "Company");
    words.emplace_back("학생", "Student");
    words.emplace_back("선생님", "Teacher");
    words.emplace_back("교수님", "Professor");
    words.emplace_back("화이트보드", "Whiteboard");
    words.emplace_back("책", "Book");
    words.emplace_back("공책, 노트", "Notebook");
    words.emplace_back("교과서", "Textbook");
    words.emplace_back("책상", "Desk");
    words.emplace_back("의자", "Chair");
    words.emplace_back("필통", "Pencil case");
    words.emplace_back("볼펜", "Ballpen");
    words.emplace_back("연필", "Pencil");
    words.emplace_back("지우개", "Eraser");
    words.emplace_back("자", "Ruler");
    words.emplace_back("가위", "Scissors");
    words.emplace_back("풀", "Glue");
    words.emplace_back("컴퓨터", "Computer");
    words.emplace_back("노트북", "Laptop");
    words.emplace_back("가방", "Bag");

    words.emplace_back("물", "Water");
    words.emplace_back("음식", "Food");
    words.emplace_back("밥", "Meal, Rice (cooked)");
    words.emplace_back("커피", "Coffee");
    words.emplace_back("차", "Tea, Car");
    words.emplace_back("시간", "Time");
    words.emplace_back("날", "Day");
    words.emplace_back("오늘", "Today");
    words.emplace_back("내일", "Tomorrow");
    words.emplace_back("어제", "Yesterday");
    words.emplace_back("돈", "Money");
    words.emplace_back("이름", "Name");
    words.emplace_back("한국", "Korea");
    words.emplace_back("한국어", "Korean (language)");
    words.emplace_back("영어", "English");
    words.emplace_back("길", "Road, Street");
    words.emplace_back("문", "Door");
    words.emplace_back("방", "Room");

    words.emplace_back("가다", "To go");
    words.emplace_back("오다", "To come");
    words.emplace_back("보다", "To see/watch");
    words.emplace_back("먹다", "To eat");
    words.emplace_back("마시다", "To drink");
    words.emplace_back("하다", "To do");
    words.emplace_back("있다", "To exist/have");
    words.emplace_back("없다", "To not exist/have");
    words.emplace_back("자다", "To sleep");
    words.emplace_back("일어나다", "To wake up");
    words.emplace_back("공부하다", "To study");
    words.emplace_back("배우다", "To learn");
    words.emplace_back("가르치다", "To teach");
    words.emplace_back("듣다", "To listen");
    words.emplace_back("말하다", "To speak");
    words.emplace_back("읽다", "To read");
    words.emplace_back("쓰다", "To write");
    words.emplace_back("사다", "To buy");
    words.emplace_back("팔다", "To sell");
    words.emplace_back("만나다", "To meet");
    words.emplace_back("좋아하다", "To like");
    words.emplace_back("알다", "To know");
    words.emplace_back("모르다", "To not know");
    words.emplace_back("만들다", "To make");
    words.emplace_back("기다리다", "To wait");
    words.emplace_back("그리다", "To draw");
    words.emplace_back("살다", "To live");
    words.emplace_back("주다", "To give");

    words.emplace_back("좋다", "Good");
    words.emplace_back("나쁘다", "Bad");
    words.emplace_back("크다", "Big");
    words.emplace_back("작다", "Small");
    words.emplace_back("많다", "Many, much");
    words.emplace_back("적다", "Few, little");
    words.emplace_back("새롭다", "New");
    words.emplace_back("오래되다", "Old (object)");
    words.emplace_back("빠르다", "Fast");
    words.emplace_back("느리다", "Slow");
    words.emplace_back("쉽다", "Easy");
    words.emplace_back("어렵다", "Difficult");
    words.emplace_back("예쁘다", "Pretty");
    words.emplace_back("행복하다", "Happy");
    words.emplace_back("피곤하다", "Tired");
    words.emplace_back("은은하다", "Subtle");

    words.emplace_back("아주", "Very");
    words.emplace_back("정말", "Really");
    words.emplace_back("잘", "Well");
    words.emplace_back("조금", "A little");
    words.emplace_back("많이", "A lot");
    words.emplace_back("빨리", "Quickly");
    words.emplace_back("천천히", "Slowly");
    words.emplace_back("항상", "Always");
    words.emplace_back("자주", "Often");
    words.emplace_back("지금", "Now");

    // Ws and Hs
    words.emplace_back("누구", "Who");
    words.emplace_back("언제", "When");
    words.emplace_back("어디", "Where");
    words.emplace_back("왜", "Why");
    words.emplace_back("무엇", "What");
    words.emplace_back("어느", "Which");
    words.emplace_back("어떻게", "How");

    // Creating word groups centered around a theme
    // Academics group
    std::vector<const WordData*>& academics = word_groups.try_emplace("Academics").first->second;
    academics.emplace_back(get_word("학교"));
    academics.emplace_back(get_word("학생"));
    academics.emplace_back(get_word("선생님"));
    academics.emplace_back(get_word("책"));
    academics.emplace_back(get_word("교수님"));
    academics.emplace_back(get_word("화이트보드"));
    academics.emplace_back(get_word("공책, 노트"));
    academics.emplace_back(get_word("교과서"));
    academics.emplace_back(get_word("책상"));
    academics.emplace_back(get_word("의자"));
    academics.emplace_back(get_word("필통"));
    academics.emplace_back(get_word("볼펜"));
    academics.emplace_back(get_word("연필"));
    academics.emplace_back(get_word("지우개"));
    academics.emplace_back(get_word("자"));
    academics.emplace_back(get_word("가위"));
    academics.emplace_back(get_word("풀"));
    academics.emplace_back(get_word("컴퓨터"));
    academics.emplace_back(get_word("노트북"));
    academics.emplace_back(get_word("가방"));

    std::vector<const WordData*>& question_words = word_groups.try_emplace("Question Words").first->second;
    question_words.emplace_back(get_word("누구"));
    question_words.emplace_back(get_word("언제"));
    question_words.emplace_back(get_word("어디"));
    question_words.emplace_back(get_word("왜"));
    question_words.emplace_back(get_word("무엇"));
    question_words.emplace_back(get_word("어느"));
    question_words.emplace_back(get_word("어떻게"));
}

const WordData* WordDatabase::get_word(const char* word)
{
    for (unsigned int i = 0; i < words.size(); ++i) {
        if (word == words[i].word) {
            return &words[i];
        }
    }

    std::cerr << "Error: \"" << word << "\" doesn't exist in the database." << std::endl;
    return nullptr;
}

const std::vector<const WordData*>* WordDatabase::get_word_group(const char* group_name)
{
    for (auto it = word_groups.begin(); it != word_groups.end(); ++it) {
        if (it->first == group_name) {
            return &it->second;
        }
    }

    std::cerr << "Error: group \"" << group_name << "\" doesn't exist in the database." << std::endl;
    return nullptr;
}
