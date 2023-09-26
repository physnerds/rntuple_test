#define AddScalar(type, name) \
type name;

#define AddArray(type, name, size) \
type name[size];


#define Generate_Arrays(structName, ...) \
struct structName{ \
 __VA_ARGS__ \
\
};

