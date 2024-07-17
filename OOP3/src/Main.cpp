#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <ctime>
#include "opencv2/opencv.hpp"//issue calling it from the CmakeList alternative call entire library to code 

// Forward declarations
class User;
class LessonContent;
class AdaptiveLearningPlatform;

// Abstract base class for learning styles
class LearningStyle {
public:
    virtual void adaptContent(LessonContent& lesson) = 0;
    virtual ~LearningStyle() {}
};

// Derived classes for specific learning styles
class VisualLearningStyle : public LearningStyle {
public:
    void adaptContent(LessonContent& lesson) override;
};

class AuditoryLearningStyle : public LearningStyle {
public:
    void adaptContent(LessonContent& lesson) override;
};

class KinestheticLearningStyle : public LearningStyle {
public:
    void adaptContent(LessonContent& lesson) override;
};

// User class
class User {
private:
    std::string name;
    std::unique_ptr<LearningStyle> learningStyle;
    std::vector<int> completedLessons;

public:
    User(const std::string& name, std::unique_ptr<LearningStyle> style)
        : name(name), learningStyle(std::move(style)) {}

    std::string getName() const { return name; }
    LearningStyle* getLearningStyle() const { return learningStyle.get(); }
    void completeLesson(int lessonId) { completedLessons.push_back(lessonId); }
    const std::vector<int>& getCompletedLessons() const { return completedLessons; }
};

// LessonContent class
class LessonContent {
private:
    int id;
    std::string topic;
    cv::Mat visualContent;
    std::string textContent;
    std::string audioContent;

public:
    LessonContent(int id, const std::string& topic, const cv::Mat& visual, 
                  const std::string& text, const std::string& audio)
        : id(id), topic(topic), visualContent(visual), textContent(text), audioContent(audio) {}

    int getId() const { return id; }
    std::string getTopic() const { return topic; }
    cv::Mat& getVisualContent() { return visualContent; }
    std::string getTextContent() const { return textContent; }
    std::string getAudioContent() const { return audioContent; }
};

// AdaptiveLearningPlatform class
class AdaptiveLearningPlatform {
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<std::unique_ptr<LessonContent>> lessons;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addLesson(std::unique_ptr<LessonContent> lesson) {
        lessons.push_back(std::move(lesson));
    }

    void adaptContent(User& user, LessonContent& lesson) {
        user.getLearningStyle()->adaptContent(lesson);
    }

    void displayLesson(const User& user, const LessonContent& lesson) {
        std::cout << "Displaying lesson for " << user.getName() << std::endl;
        std::cout << "Topic: " << lesson.getTopic() << std::endl;
        
        cv::imshow("Visual Content", lesson.getVisualContent());
        cv::waitKey(0);
        
        std::cout << "Text Content: " << lesson.getTextContent() << std::endl;
        std::cout << "Audio Content: " << lesson.getAudioContent() << std::endl;
    }

    void trackProgress(User& user, int lessonId) {
        user.completeLesson(lessonId);
    }

    std::vector<LessonContent*> recommendLessons(const User& user) {
        std::vector<LessonContent*> recommendations;
        const auto& completedLessons = user.getCompletedLessons();
        
        for (const auto& lesson : lessons) {
            if (std::find(completedLessons.begin(), completedLessons.end(), lesson->getId()) == completedLessons.end()) {
                recommendations.push_back(lesson.get());
            }
        }
        
        return recommendations;
    }
};

// Implementation of learning style adaptations
void VisualLearningStyle::adaptContent(LessonContent& lesson) {
    cv::GaussianBlur(lesson.getVisualContent(), lesson.getVisualContent(), cv::Size(5, 5), 0);
}

void AuditoryLearningStyle::adaptContent(LessonContent& lesson) {
    // Implement audio enhancement logic here
}

void KinestheticLearningStyle::adaptContent(LessonContent& lesson) {
    // Implement interactive content adaptation here
}

// Custom exception class
class LessonNotFoundException : public std::runtime_error {
public:
    LessonNotFoundException(const std::string& message) : std::runtime_error(message) {}
};

// Operator overloading example
bool operator==(const User& lhs, const User& rhs) {
    return lhs.getName() == rhs.getName();
}

// Function overloading example
void displayUserInfo(const User& user) {
    std::cout << "User: " << user.getName() << std::endl;
}

void displayUserInfo(const User& user, const std::vector<LessonContent*>& recommendations) {
    displayUserInfo(user);
    std::cout << "Recommended lessons:" << std::endl;
    for (const auto& lesson : recommendations) {
        std::cout << "- " << lesson->getTopic() << std::endl;
    }
}

int main() {
    try {
        AdaptiveLearningPlatform platform;

        // Create users with different learning styles
        platform.addUser(std::make_unique<User>("Alice", std::make_unique<VisualLearningStyle>()));
        platform.addUser(std::make_unique<User>("Bob", std::make_unique<AuditoryLearningStyle>()));
        platform.addUser(std::make_unique<User>("Charlie", std::make_unique<KinestheticLearningStyle>()));

        // Create lessons
        platform.addLesson(std::make_unique<LessonContent>(1, "Introduction to Fractions", 
            cv::imread("fraction_diagram.jpg"), "Fractions represent parts of a whole...", "Audio explanation of fractions"));
        platform.addLesson(std::make_unique<LessonContent>(2, "Basic Algebra", 
            cv::imread("algebra_equations.jpg"), "Algebra is the study of mathematical symbols...", "Audio explanation of algebra"));

        // Simulate user interaction
        User* currentUser = platform.getUsers()[0].get();
        LessonContent* currentLesson = platform.getLessons()[0].get();

        platform.adaptContent(*currentUser, *currentLesson);
        platform.displayLesson(*currentUser, *currentLesson);
        platform.trackProgress(*currentUser, currentLesson->getId());

        // Demonstrate exception handling
        try {
            LessonContent* nonExistentLesson = nullptr;
            platform.displayLesson(*currentUser, *nonExistentLesson);
        } catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Demonstrate operator and function overloading
        User* anotherUser = platform.getUsers()[1].get();
        if (*currentUser == *anotherUser) {
            std::cout << "Users are the same." << std::endl;
        } else {
            std::cout << "Users are different." << std::endl;
        }

        auto recommendations = platform.recommendLessons(*currentUser);
        displayUserInfo(*currentUser, recommendations);

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}