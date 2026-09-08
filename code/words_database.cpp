#include "words_database.hpp"
#include "gamemode_manager.hpp"
#include <ctime>
#include <iostream>
#include <random>
#include <utility>

std::vector<WordData>                                     WordDatabase::words;
std::map<const std::string, std::vector<const WordData*>> WordDatabase::word_groups;

void WordDatabase::init()
{
    GamemodeManager::GamemodeSettings::selection[0] = std::vector<const char*>{ "KR -> EN", "EN -> KR" };
    GamemodeManager::GamemodeSettings::selection[1] = std::vector<const char*>{ "EASY", "EASY+", "MEDIUM", "MEDIUM+", "HARD" };
    GamemodeManager::GamemodeSettings::selection[2] = std::vector<const char*>{};
    GamemodeManager::GamemodeSettings::selection[2].reserve(4);

    WordData KR_EN_pairs[] = {
        // Populating word database
        // Pronouns section
        { { "나", "제"   }, { "I"          } },
        { { "너", "당신" }, { "You"        } },
        { { "우리"       }, { "We, our"    } },
        { { "그"         }, { "He"         } },
        { { "그녀"       }, { "She"        } },
        { { "이것"       }, { "This thing" } },
        { { "그것"       }, { "That thing" } },

        // Nouns section
        { { "사람" }, { "Person"          } },
        { { "친구" }, { "Friend"          } },
        { { "가족" }, { "Family"          } },
        { { "엄마" }, { "Mom"             } },
        { { "아빠" }, { "Dad"             } },
        { { "아이" }, { "Child"           } },
        { { "집"   }, { "House"           } },
        { { "옆집" }, { "Next Door House" } },
        { { "방"   }, { "Room"            } },
        { { "문"   }, { "Door"            } },
        { { "길"   }, { "Road", "Street"  } },

        { { "학교"         }, { "School"      } },
        { { "회사"         }, { "Company"     } },
        { { "학생"         }, { "Student"     } },
        { { "선생님"       }, { "Teacher"     } },
        { { "교수님"       }, { "Professor"   } },
        { { "화이트보드"   }, { "Whiteboard"  } },
        { { "책"           }, { "Book"        } },
        { { "공책", "노트" }, { "Notebook"    } },
        { { "교과서"       }, { "Textbook"    } },
        { { "책상"         }, { "Desk"        } },
        { { "의자"         }, { "Chair"       } },
        { { "필통"         }, { "Pencil Case" } },
        { { "볼펜"         }, { "Ballpen"     } },
        { { "연필"         }, { "Pencil"      } },
        { { "지우개"       }, { "Eraser"      } },
        { { "자"           }, { "Ruler"       } },
        { { "가위"         }, { "Scissors"    } },
        { { "풀"           }, { "Glue"        } },
        { { "컴퓨터"       }, { "Computer"    } },
        { { "노트북"       }, { "Laptop"      } },
        { { "가방"         }, { "Bag"         } },

        // Foods and drinks
        { { "물"   }, { "Water"                } },
        { { "음식" }, { "Food"                 } },
        { { "밥"   }, { "Meal", "Cooked Rice " } },
        { { "커피" }, { "Coffee"               } },
        { { "차"   }, { "Tea", "Car"           } },

        // Temporal nouns
        { { "초"         }, { "Second (Time)"    } },
        { { "분"         }, { "Minute"           } },
        { { "시간"       }, { "Hour", "Time"     } },
        { { "일"         }, { "Day (Counter)"    } },
        { { "날"         }, { "Day (General)"    } },
        { { "주", "주일" }, { "Week"             } },
        { { "달", "개월" }, { "Month (Counter)"  } },
        { { "월"         }, { "Month (Calendar)" } },
        { { "년"         }, { "Year (Counter)"   } },
        { { "해"         }, { "Year (General)"   } },
        { { "오늘"       }, { "Today"            } },
        { { "내일"       }, { "Tomorrow"         } },
        { { "어제"       }, { "Yesterday"        } },
        { { "일요일"     }, { "Sunday"           } },
        { { "월요일"     }, { "Monday"           } },
        { { "화요일"     }, { "Tuesday"          } },
        { { "수요일"     }, { "Wednesday"        } },
        { { "목요일"     }, { "Thursday"         } },
        { { "금요일"     }, { "Friday"           } },
        { { "도요일"     }, { "Saturday"         } },
        { { "일월"       }, { "January"          } },
        { { "이월"       }, { "February"         } },
        { { "삼월"       }, { "March"            } },
        { { "사월"       }, { "April"            } },
        { { "오월"       }, { "May"              } },
        { { "유월"       }, { "June"             } },
        { { "칠월"       }, { "July"             } },
        { { "탈월"       }, { "August"           } },
        { { "구월"       }, { "September"        } },
        { { "시월"       }, { "October"          } },
        { { "십일월"     }, { "November"         } },
        { { "십이월"     }, { "December"         } },

        // Countries
        { { "한국" }, { "Korea" } },

        // Generic things
        { { "돈"   }, { "Money" } },
        { { "이름" }, { "Name"  } },

        // Abstract concepts
        { { "한국어" }, { "Korean"  } },
        { { "영어"   }, { "English" } },

        // Verbs
        { { "가다"     }, { "To go"                       } },
        { { "오다"     }, { "To come"                     } },
        { { "보다"     }, { "To see", "To watch"          } },
        { { "먹다"     }, { "To eat"                      } },
        { { "마시다"   }, { "To drink"                    } },
        { { "하다"     }, { "To do"                       } },
        { { "있다"     }, { "To exist", "To have"         } },
        { { "없다"     }, { "To not exist", "To not have" } },
        { { "자다"     }, { "To sleep"                    } },
        { { "일어나다" }, { "To wake up"                  } },
        { { "공부하다" }, { "To study"                    } },
        { { "배우다"   }, { "To learn"                    } },
        { { "가르치다" }, { "To teach"                    } },
        { { "듣다"     }, { "To listen"                   } },
        { { "말하다"   }, { "To speak"                    } },
        { { "읽다"     }, { "To read"                     } },
        { { "쓰다"     }, { "To write"                    } },
        { { "사다"     }, { "To buy"                      } },
        { { "팔다"     }, { "To sell"                     } },
        { { "만나다"   }, { "To meet"                     } },
        { { "좋아하다" }, { "To like"                     } },
        { { "알다"     }, { "To know"                     } },
        { { "모르다"   }, { "To not know"                 } },
        { { "만들다"   }, { "To make"                     } },
        { { "기다리다" }, { "To wait"                     } },
        { { "그리다"   }, { "To draw"                     } },
        { { "살다"     }, { "To live"                     } },
        { { "주다"     }, { "To give"                     } },

        // Modifiers
        { { "좋다"        }, { "Good"           } },
        { { "나쁘다"      }, { "Bad"            } },
        { { "크다"        }, { "Big"            } },
        { { "작다"        }, { "Small"          } },
        { { "많다"        }, { "Many", "Plenty" } },
        { { "적다"        }, { "Little", "Few"  } },
        { { "새롭다"      }, { "New"            } },
        { { "젊다"        }, { "Young"          } },
        { { "오래되다"    }, { "Old (object)"   } },
        { { "늙다"        }, { "Old (people)"   } },
        { { "빠르다"      }, { "Fast"           } },
        { { "느리다"      }, { "Slow"           } },
        { { "쉽다"        }, { "Easy"           } },
        { { "어렵다"      }, { "Difficult"      } },
        { { "잘생기다"    }, { "Handsome"       } },
        { { "아름답다"    }, { "Beautiful"      } },
        { { "예쁘다"      }, { "Pretty"         } },
        { { "귀엽다"      }, { "Cute"           } },
        { { "못생기다"    }, { "Ugly"           } },
        { { "행복하다"    }, { "Happy"          } },
        { { "슬프다"      }, { "Sad"            } },
        { { "화나다"      }, { "Angry"          } },
        { { "피곤하다"    }, { "Tired"          } },
        { { "은은하다"    }, { "Subtle"         } },
        { { "부드럽다"    }, { "Soft"           } },
        { { "딱딱하다"    }, { "Hard"           } },
        { { "길다"        }, { "Long"           } },
        { { "짧다"        }, { "Short (Length)" } },
        { { "높다"        }, { "Tall"           } },
        { { "낮다"        }, { "Short (Height)" } },
        { { "넓다"        }, { "Wide"           } },
        { { "좁다"        }, { "Narrow"         } },
        { { "두껍다"      }, { "Thick"          } },
        { { "얇다"        }, { "Thin"           } },
        { { "무겁다"      }, { "Heavy"          } },
        { { "가볍다"      }, { "Light"          } },
        { { "강하다"      }, { "Strong"         } },
        { { "약하다"      }, { "Weak"           } },
        { { "밝다"        }, { "Bright"         } },
        { { "어둡다"      }, { "Dark"           } },
        { { "뜨겁다"      }, { "Hot"            } },
        { { "따뜻하다"    }, { "Warm"           } },
        { { "차갑다"      }, { "Cold"           } },
        { { "개뜻하아"    }, { "Clean"          } },
        { { "더럽다"      }, { "Dirty"          } },
        { { "싸다"        }, { "Cheap"          } },
        { { "비싸다"      }, { "Expensive"      } },
        { { "가난하다"    }, { "Poor"           } },
        { { "부유하다"    }, { "Rich"           } },
        { { "운이 좋다"   }, { "Lucky"          } },
        { { "운이 나쁘다" }, { "Unlucky"        } },

        { { "아주"   }, { "Very"     } },
        { { "정말"   }, { "Really"   } },
        { { "잘"     }, { "Well"     } },
        { { "조금"   }, { "A little" } },
        { { "많이"   }, { "A lot"    } },
        { { "빨리"   }, { "Quickly"  } },
        { { "천천히" }, { "Slowly"   } },
        { { "항상"   }, { "Always"   } },
        { { "자주"   }, { "Often"    } },
        { { "지금"   }, { "Now"      } },

        // Ws and Hs
        { { "누구"       }, { "Who"   } },
        { { "언제"       }, { "When"  } },
        { { "어디"       }, { "Where" } },
        { { "왜"         }, { "Why"   } },
        { { "무엇", "뭐" }, { "What"  } },
        { { "어느"       }, { "Which" } },
        { { "어떻게"     }, { "How"   } },
    };

    for (const WordData& pair : KR_EN_pairs) {
        words.emplace_back(std::move(pair));
    }

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
    academics.emplace_back(get_word("공책"));
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
    temporal_nouns.emplace_back(get_word("주"));
    temporal_nouns.emplace_back(get_word("달"));
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

    // Randomized word group that resets daily
    std::vector<const WordData*>& random = word_groups.try_emplace("Random").first->second;
    GamemodeManager::GamemodeSettings::selection[2].emplace_back("Random");

    auto now =  std::time(nullptr);
    auto lt  = *std::localtime(&now);

    unsigned int rd_seed = 10000 * (lt.tm_year + 1900) + 100 * (lt.tm_mon + 1) + lt.tm_mday;

    std::mt19937                       gen(rd_seed);
    std::uniform_int_distribution<int> dist(0, words.size() - 1);

    unsigned int rd_items[50];

    for (unsigned int i = 0; i < 50; ++i) {
        unsigned int n = dist(gen);

        while (true) {
            bool is_dupe = false;

            for (unsigned int j = 0; j < i; ++j) {
                if (n == rd_items[j]) {
                    is_dupe = true;
                    break;
                }
            }

            if (!is_dupe) {
                break;
            }

            n = dist(gen);
        }

        rd_items[i] = n;
    }

    for (unsigned int i = 0; i < 50; ++i) {
        const char* key = KR_EN_pairs[rd_items[i]].KR[0].c_str();

        random.emplace_back(get_word(key));
    }
}

const WordData* WordDatabase::get_word(const char* word)
{
    for (const WordData& word_data : words) {
        for (const std::string& key : word_data.KR) {
            if (word == key) {
                return &word_data;
            }
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
