#include "words_database.hpp"
#include "gamemode_manager.hpp"
#include <iostream>

std::vector<WordData> WordDatabase::words;
std::map<const std::string, std::vector<const WordData*>> WordDatabase::word_groups;

void WordDatabase::init()
{
    GamemodeManager::GamemodeSettings::selection[0] = std::vector<const char*>{ "KR -> EN", "EN -> KR" };
    GamemodeManager::GamemodeSettings::selection[1] = std::vector<const char*>{ "EASY", "EASY+", "MEDIUM", "MEDIUM+", "HARD" };
    GamemodeManager::GamemodeSettings::selection[2] = std::vector<const char*>{};
    GamemodeManager::GamemodeSettings::selection[2].reserve(4);

    // Populating word database
    // Pronouns section
    words.emplace_back("나 / 제", "I");
    words.emplace_back("너 / 당신", "You");
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
    words.emplace_back("집", "House");
    words.emplace_back("옆집", "Next Door House");
    words.emplace_back("방", "Room");
    words.emplace_back("문", "Door");
    words.emplace_back("길", "Road / Street");

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
    words.emplace_back("필통", "Pencil Case");
    words.emplace_back("볼펜", "Ballpen");
    words.emplace_back("연필", "Pencil");
    words.emplace_back("지우개", "Eraser");
    words.emplace_back("자", "Ruler");
    words.emplace_back("가위", "Scissors");
    words.emplace_back("풀", "Glue");
    words.emplace_back("컴퓨터", "Computer");
    words.emplace_back("노트북", "Laptop");
    words.emplace_back("가방", "Bag");

    // Foods and drinks
    words.emplace_back("물", "Water");
    words.emplace_back("음식", "Food");
    words.emplace_back("밥", "Meal / Cooked Rice ");
    words.emplace_back("커피", "Coffee");
    words.emplace_back("차", "Tea / Car");

    // Temporal nouns
    words.emplace_back("초", "Second (Time)");
    words.emplace_back("분", "Minute");
    words.emplace_back("시간", "Hour / Time");
    words.emplace_back("일", "Day (Counter)");
    words.emplace_back("날", "Day (General)");
    words.emplace_back("주/주일", "Week");
    words.emplace_back("달/개월", "Month (Counter)");
    words.emplace_back("월", "Month (Calendar)");
    words.emplace_back("년", "Year (Counter)");
    words.emplace_back("해", "Year (General)");
    words.emplace_back("오늘", "Today");
    words.emplace_back("내일", "Tomorrow");
    words.emplace_back("어제", "Yesterday");
    words.emplace_back("일요일", "Sunday");
    words.emplace_back("월요일", "Monday");
    words.emplace_back("화요일", "Tuesday");
    words.emplace_back("수요일", "Wednesday");
    words.emplace_back("목요일", "Thursday");
    words.emplace_back("금요일", "Friday");
    words.emplace_back("도요일", "Saturday");
    words.emplace_back("일월", "January");
    words.emplace_back("이월", "February");
    words.emplace_back("삼월", "March");
    words.emplace_back("사월", "April");
    words.emplace_back("오월", "May");
    words.emplace_back("유월", "June");
    words.emplace_back("칠월", "July");
    words.emplace_back("탈월", "August");
    words.emplace_back("구월", "September");
    words.emplace_back("시월", "October");
    words.emplace_back("십일월", "November");
    words.emplace_back("십이월", "December");

    // Countries
    words.emplace_back("한국", "Korea");

    // Generic things
    words.emplace_back("돈", "Money");
    words.emplace_back("이름", "Name");

    // Abstract concepts
    words.emplace_back("한국어", "Korean");
    words.emplace_back("영어", "English");

    // Verbs
    words.emplace_back("가다", "To go");
    words.emplace_back("오다", "To come");
    words.emplace_back("보다", "To see / watch");
    words.emplace_back("먹다", "To eat");
    words.emplace_back("마시다", "To drink");
    words.emplace_back("하다", "To do");
    words.emplace_back("있다", "To exist / have");
    words.emplace_back("없다", "To not exist / have");
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

    // Modifiers
    words.emplace_back("좋다", "Good");
    words.emplace_back("나쁘다", "Bad");
    words.emplace_back("크다", "Big");
    words.emplace_back("작다", "Small");
    words.emplace_back("많다", "Many / Plentiful");
    words.emplace_back("적다", "Little / Few");
    words.emplace_back("새롭다", "New");
    words.emplace_back("젊다", "Young");
    words.emplace_back("오래되다", "Old (object)");
    words.emplace_back("늙다", "Old (people)");
    words.emplace_back("빠르다", "Fast");
    words.emplace_back("느리다", "Slow");
    words.emplace_back("쉽다", "Easy");
    words.emplace_back("어렵다", "Difficult");
    words.emplace_back("잘생기다", "Handsome");
    words.emplace_back("아름답다", "Beautiful");
    words.emplace_back("예쁘다", "Pretty");
    words.emplace_back("귀엽다", "Cute");
    words.emplace_back("못생기다", "Ugly");
    words.emplace_back("행복하다", "Happy");
    words.emplace_back("슬프다", "Sad");
    words.emplace_back("화나다", "Angry");
    //words.emplace_back("", "Energetic");
    words.emplace_back("피곤하다", "Tired");
    //words.emplace_back("", "");
    words.emplace_back("은은하다", "Subtle");
    words.emplace_back("부드럽다", "Soft");
    words.emplace_back("딱딱하다", "Hard");
    words.emplace_back("길다", "Long");
    words.emplace_back("짧다", "Short (Length)");
    words.emplace_back("높다", "Tall");
    words.emplace_back("낮다", "Short (Height)");
    words.emplace_back("넓다", "Wide");
    words.emplace_back("좁다", "Narrow");
    words.emplace_back("두껍다", "Thick");
    words.emplace_back("얇다", "Thin");
    words.emplace_back("무겁다", "Heavy");
    words.emplace_back("가볍다", "Light");
    words.emplace_back("강하다", "Strong");
    words.emplace_back("약하다", "Weak");
    words.emplace_back("밝다", "Bright");
    words.emplace_back("어둡다", "Dark");
    words.emplace_back("뜨겁다", "Hot");
    words.emplace_back("따뜻하다", "Warm");
    words.emplace_back("차갑다", "Cold");
    words.emplace_back("개뜻하아", "Clean");
    words.emplace_back("더럽다", "Dirty");
    words.emplace_back("싸다", "Cheap");
    words.emplace_back("비싸다", "Expensive");
    words.emplace_back("가난하다", "Poor");
    words.emplace_back("부유하다", "Rich");
    words.emplace_back("운이 좋다", "Lucky");
    words.emplace_back("운이 나쁘다", "Unlucky");

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
    GamemodeManager::GamemodeSettings::selection[2].emplace_back("Academics");
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
    GamemodeManager::GamemodeSettings::selection[2].emplace_back("Question Words");
    question_words.emplace_back(get_word("누구"));
    question_words.emplace_back(get_word("언제"));
    question_words.emplace_back(get_word("어디"));
    question_words.emplace_back(get_word("왜"));
    question_words.emplace_back(get_word("무엇"));
    question_words.emplace_back(get_word("어느"));
    question_words.emplace_back(get_word("어떻게"));

    std::vector<const WordData*>& temporal_nouns = word_groups.try_emplace("Temporal Nouns").first->second;
    GamemodeManager::GamemodeSettings::selection[2].emplace_back("Temporal Nouns");
    temporal_nouns.emplace_back(get_word("시간"));
    temporal_nouns.emplace_back(get_word("초"));
    temporal_nouns.emplace_back(get_word("분"));
    temporal_nouns.emplace_back(get_word("시간"));
    temporal_nouns.emplace_back(get_word("일"));
    temporal_nouns.emplace_back(get_word("날"));
    temporal_nouns.emplace_back(get_word("주/주일"));
    temporal_nouns.emplace_back(get_word("달/개월"));
    temporal_nouns.emplace_back(get_word("월"));
    temporal_nouns.emplace_back(get_word("년"));
    temporal_nouns.emplace_back(get_word("해"));
    temporal_nouns.emplace_back(get_word("오늘"));
    temporal_nouns.emplace_back(get_word("내일"));
    temporal_nouns.emplace_back(get_word("어제"));
    temporal_nouns.emplace_back(get_word("일요일"));
    temporal_nouns.emplace_back(get_word("월요일"));
    temporal_nouns.emplace_back(get_word("화요일"));
    temporal_nouns.emplace_back(get_word("수요일"));
    temporal_nouns.emplace_back(get_word("목요일"));
    temporal_nouns.emplace_back(get_word("금요일"));
    temporal_nouns.emplace_back(get_word("도요일"));
    temporal_nouns.emplace_back(get_word("일월"));
    temporal_nouns.emplace_back(get_word("이월"));
    temporal_nouns.emplace_back(get_word("삼월"));
    temporal_nouns.emplace_back(get_word("사월"));
    temporal_nouns.emplace_back(get_word("오월"));
    temporal_nouns.emplace_back(get_word("유월"));
    temporal_nouns.emplace_back(get_word("칠월"));
    temporal_nouns.emplace_back(get_word("탈월"));
    temporal_nouns.emplace_back(get_word("구월"));
    temporal_nouns.emplace_back(get_word("시월"));
    temporal_nouns.emplace_back(get_word("십일월"));
    temporal_nouns.emplace_back(get_word("십이월"));

    std::vector<const WordData*>& modifiers_1 = word_groups.try_emplace("Modifiers 1").first->second;
    GamemodeManager::GamemodeSettings::selection[2].emplace_back("Modifiers 1");
    modifiers_1.emplace_back(get_word("좋다"));
    modifiers_1.emplace_back(get_word("나쁘다"));
    modifiers_1.emplace_back(get_word("크다"));
    modifiers_1.emplace_back(get_word("작다"));
    modifiers_1.emplace_back(get_word("많다"));
    modifiers_1.emplace_back(get_word("적다"));
    modifiers_1.emplace_back(get_word("새롭다"));
    modifiers_1.emplace_back(get_word("젊다"));
    modifiers_1.emplace_back(get_word("오래되다"));
    modifiers_1.emplace_back(get_word("늙다"));
    modifiers_1.emplace_back(get_word("빠르다"));
    modifiers_1.emplace_back(get_word("느리다"));
    modifiers_1.emplace_back(get_word("쉽다"));
    modifiers_1.emplace_back(get_word("어렵다"));
    modifiers_1.emplace_back(get_word("잘생기다"));
    modifiers_1.emplace_back(get_word("아름답다"));
    modifiers_1.emplace_back(get_word("예쁘다"));
    modifiers_1.emplace_back(get_word("귀엽다"));
    modifiers_1.emplace_back(get_word("못생기다"));
    modifiers_1.emplace_back(get_word("행복하다"));
    modifiers_1.emplace_back(get_word("슬프다"));
    modifiers_1.emplace_back(get_word("화나다"));
    modifiers_1.emplace_back(get_word("부드럽다"));
    modifiers_1.emplace_back(get_word("딱딱하다"));
    modifiers_1.emplace_back(get_word("길다"));
    modifiers_1.emplace_back(get_word("짧다"));
    modifiers_1.emplace_back(get_word("높다"));
    modifiers_1.emplace_back(get_word("낮다"));
    modifiers_1.emplace_back(get_word("넓다"));
    modifiers_1.emplace_back(get_word("좁다"));
    modifiers_1.emplace_back(get_word("두껍다"));
    modifiers_1.emplace_back(get_word("얇다"));
    modifiers_1.emplace_back(get_word("무겁다"));
    modifiers_1.emplace_back(get_word("가볍다"));
    modifiers_1.emplace_back(get_word("강하다"));
    modifiers_1.emplace_back(get_word("약하다"));
    modifiers_1.emplace_back(get_word("밝다"));
    modifiers_1.emplace_back(get_word("어둡다"));
    modifiers_1.emplace_back(get_word("뜨겁다"));
    modifiers_1.emplace_back(get_word("따뜻하다"));
    modifiers_1.emplace_back(get_word("차갑다"));
    modifiers_1.emplace_back(get_word("개뜻하아"));
    modifiers_1.emplace_back(get_word("더럽다"));
    modifiers_1.emplace_back(get_word("싸다"));
    modifiers_1.emplace_back(get_word("비싸다"));
    modifiers_1.emplace_back(get_word("가난하다"));
    modifiers_1.emplace_back(get_word("부유하다"));
    modifiers_1.emplace_back(get_word("운이 좋다"));
    modifiers_1.emplace_back(get_word("운이 나쁘다"));
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

const std::vector<const char*> WordDatabase::get_keys()
{
    std::vector<const char*> keys;
    keys.reserve(word_groups.size());

    for (const auto& [key, value] : word_groups) {
        keys.push_back(key.c_str());
    }

    return keys;
}
