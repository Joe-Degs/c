struct studentT {
    char name[64];
    int age;
    int grad_yr;
    float gpa;
};

struct studentT student1; // declaration but is relatively long
			  // requires struct in every declaration.

// you can give aliases to struct types to avoid using
// the whole struct <struct_name> when declaring types.
// Just like go's type aliasing, this one also works the same
// ("Student" is alias for "struct studentT")
typedef struct studentT Student;

// so to define a type, you use
Student std1; // simple declaration
Student std2[50] // array of 50 studentTs

//void change_student_name(struct studentT* std, char name)
