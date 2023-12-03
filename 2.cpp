#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

enum Specialty
{
    COMPUTER_SCIENCE,
    INFORMATICS,
    MATH_ECON,
    PHYS_INF,
    HANDICRAFT
};

union ThirdGrade
{
    double programmingGrade;      // COMPUTER_SCIENCE
    double numericalMethodsGrade; // INFORMATICS
    double pedagogyGrade;         // MATH_ECON, PHYS_INF, HANDICRAFT
};

struct Student
{
    int id;
    string lastName;
    int course;
    Specialty specialty;
    double physicsGrade;
    double mathGrade;
    ThirdGrade thirdGrade;
};

const char *specialtyStr[] = {"computer science", "informatics", "math and economics", "physics and informatics", "handicraft"};

void Create(Student &student, int id);
void processUserChoice(Student *students, int numberOfStudents, int *indexArray);
void searchStudent(const Student *students, const int *indexArray, int numberOfStudents);
void printTable(const Student *students, int n);
void sortStudents(Student *students, int n, int *indexArray);
bool binarySearch(const Student *students, const int *indexArray, int n, const string &searchLastName, Specialty searchSpecialty, int searchCourse);
void PrintIndexSorted(Student *students, int *I, const int numberOfStudents);
int *IndexSort(Student *students, const int numberOfStudents);

int main()
{
    int numberOfStudents;
    cout << "Введіть кількість студентів: ";
    cin >> numberOfStudents;

    Student *students = new Student[numberOfStudents];
    int *indexArray = new int[numberOfStudents];

    processUserChoice(students, numberOfStudents, indexArray);

    delete[] students;
    delete[] indexArray;

    return 0;
}

void Create(Student &student, int id)
{
    cout << "Студент #" << id << ":\n";
    cin.ignore();

    cout << "Прізвище: ";
    getline(cin, student.lastName);

    cout << "Курс: ";
    cin >> student.course;

    cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
    int specialtyValue;
    cin >> specialtyValue;

    student.specialty = static_cast<Specialty>(specialtyValue);

    cout << "Оцінка з фізики: ";
    cin >> student.physicsGrade;

    cout << "Оцінка з математики: ";
    cin >> student.mathGrade;

    if (student.specialty == COMPUTER_SCIENCE)
    {
        cout << "Оцінка з програмування: ";
        cin >> student.thirdGrade.programmingGrade;
    }
    else if (student.specialty == INFORMATICS)
    {
        cout << "Оцінка з чисельних методів: ";
        cin >> student.thirdGrade.numericalMethodsGrade;
    }
    else
    {
        cout << "Оцінка з педагогіки: ";
        cin >> student.thirdGrade.pedagogyGrade;
    }

    // student.id = id;

    cout << endl;
}

void printTable(const Student *students, int n)
{
    cout << "==============================================================================================================================="
         << endl;
    cout << "| №  | Прізвище | Курс | Спеціальність            | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |"
         << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------------"
         << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "| " << setw(2) << right << i + 1 << " "
             << "| " << setw(9) << left << students[i].lastName
             << "| " << setw(5) << right << students[i].course
             << "| " << setw(25) << left << specialtyStr[static_cast<int>(students[i].specialty)]
             << "| " << setw(7) << right << students[i].physicsGrade
             << "| " << setw(11) << right << students[i].mathGrade;
        switch (students[i].specialty)
        {
        case COMPUTER_SCIENCE:
            cout << "| " << setw(13) << right << students[i].thirdGrade.programmingGrade
                 << " |                 |            |";
            break;
        case INFORMATICS:
            cout << "|               | " << setw(15) << right << students[i].thirdGrade.numericalMethodsGrade
                 << " |            |";
            break;
        default:
            cout << "|               |                 | " << setw(10) << right << students[i].thirdGrade.pedagogyGrade << " |";
            break;
        }

        cout << endl;
    }
    cout << "==============================================================================================================================="
         << endl;
    cout << endl;
}

void sortStudents(Student *students, const int N, int *indexArray)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if ((students[indexArray[i1]].specialty > students[indexArray[i1 + 1]].specialty) ||
                (students[indexArray[i1]].specialty == students[indexArray[i1 + 1]].specialty &&
                 students[indexArray[i1]].course > students[indexArray[i1 + 1]].course) ||
                (students[indexArray[i1]].specialty == students[indexArray[i1 + 1]].specialty &&
                 students[indexArray[i1]].course == students[indexArray[i1 + 1]].course &&
                 students[indexArray[i1]].lastName > students[indexArray[i1 + 1]].lastName))
            {
                tmp = students[indexArray[i1]];
                students[indexArray[i1]] = students[indexArray[i1 + 1]];
                students[indexArray[i1 + 1]] = tmp;

                int tempIndex = indexArray[i1];
                indexArray[i1] = indexArray[i1 + 1];
                indexArray[i1 + 1] = tempIndex;
            }
}

bool binarySearch(const Student *students, const int *indexArray, int n, const string &searchLastName, Specialty searchSpecialty, int searchCourse)
{
    int left = 0, right = n - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int studentIndex = indexArray[mid];

        if (students[studentIndex].specialty == searchSpecialty &&
            students[studentIndex].course == searchCourse &&
            students[studentIndex].lastName == searchLastName)
        {
            return true;
        }

        if (students[studentIndex].specialty < searchSpecialty ||
            (students[studentIndex].specialty == searchSpecialty && students[studentIndex].course < searchCourse) ||
            (students[studentIndex].specialty == searchSpecialty && students[studentIndex].course == searchCourse && students[studentIndex].lastName < searchLastName))
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return false;
}

void processUserChoice(Student *students, int numberOfStudents, int *indexArray)
{
    int choice;
    do
    {
        cout << "Меню:\n";
        cout << "1. Введення даних з клавіатури\n";
        cout << "2. Вивід даних на екран\n";
        cout << "3. Впорядкування за спеціальністю, курсом та прізвищем\n";
        cout << "4. Пошук студента за прізвищем, спеціальністю та курсом\n";
        cout << "5. Індексне впорядкування\n";
        cout << "6. Вихід\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            for (int i = 0; i < numberOfStudents; ++i)
            {
                Create(students[i], i + 1);
                indexArray[i] = i;
            }
            break;
        case 2:
            printTable(students, numberOfStudents);
            break;
        case 3:
            sortStudents(students, numberOfStudents, indexArray);
            printTable(students, numberOfStudents);
            break;
        case 4:
            searchStudent(students, indexArray, numberOfStudents);
            break;
        case 5:
            PrintIndexSorted(students, IndexSort(students, numberOfStudents), numberOfStudents);
            break;
        case 6:
            cout << "Програма завершена!\n";
            break;
        default:
            cout << "Неправильний вибір. Спробуйте ще раз.\n";
            break;
        }
    } while (choice != 6);
}

void searchStudent(const Student *students, const int *indexArray, int numberOfStudents)
{
    cin.ignore();
    string searchLastName;
    cout << "Введіть прізвище: ";
    getline(cin, searchLastName);

    int searchSpecialty;
    cout << "Введіть спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
    cin >> searchSpecialty;

    int searchCourse;
    cout << "Введіть курс: ";
    cin >> searchCourse;

    if (binarySearch(students, indexArray, numberOfStudents, searchLastName, static_cast<Specialty>(searchSpecialty), searchCourse))
    {
        cout << "Студент знайдений.\n";
    }
    else
    {
        cout << "Студент не знайдений.\n";
    }
}

int *IndexSort(Student *students, const int numberOfStudents)
{
    int *I = new int[numberOfStudents];
    for (int i = 0; i < numberOfStudents; i++)
        I[i] = i;
    int i, j, value;
    for (i = 1; i < numberOfStudents; i++)
    {
        value = I[i];
        for (j = i - 1;
             j >= 0 && ((students[I[j]].specialty > students[value].specialty) ||
                        (students[I[j]].specialty == students[value].specialty &&
                         students[I[j]].course > students[value].course) ||
                        (students[I[j]].specialty == students[value].specialty &&
                         students[I[j]].course == students[value].course &&
                         students[I[j]].lastName > students[value].lastName));
             j--)
        {
            I[j + 1] = I[j];
        }
        I[j + 1] = value;
    }
    return I;
}

void PrintIndexSorted(Student *students, int *I, const int numberOfStudents)
{
    cout << "==============================================================================================================================="
         << endl;
    cout << "| №  | Прізвище | Курс | Спеціальність            | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |"
         << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------------"
         << endl;
    for (int i = 0; i < numberOfStudents; i++)
    {
        cout << "| " << setw(2) << right << i + 1 << " ";
        cout << "| " << setw(9) << left << students[I[i]].lastName
             << "| " << setw(4) << right << students[I[i]].course << " "
             << "| " << setw(25) << left << specialtyStr[students[I[i]].specialty]
             << "| " << setw(6) << right << students[I[i]].physicsGrade << " "
             << "| " << setw(10) << right << students[I[i]].mathGrade << " ";
        switch (students[i].specialty)
        {
        case COMPUTER_SCIENCE:
            cout << "| " << setw(13) << right << students[i].thirdGrade.programmingGrade
                 << " |                 |            |";
            break;
        case INFORMATICS:
            cout << "|               | " << setw(15) << right << students[i].thirdGrade.numericalMethodsGrade
                 << " |            |";
            break;
        default:
            cout << "|               |                 | " << setw(10) << right << students[i].thirdGrade.pedagogyGrade << " |";
            break;
        }

        cout << endl;
    }
    cout << "==============================================================================================================================="
         << endl;
    cout << endl;
}