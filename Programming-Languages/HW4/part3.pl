%facts

enrollment(a,102).
enrollment(a,108).
enrollment(b,102).
enrollment(c,108).
enrollment(d,341).
enrollment(e,455).

whens(102, 10).
whens(108, 12).
whens(341, 14).
whens(455, 16).
whens(452, 17).

where(102, z23).
where(108, z11).
where(341, z06).
where(455, 207).
where(452, 207).


%rules

schedule(Student, P, T) :- enrollment(Student, Class), where(Class, P), whens(Class,T).

usage(Room, T) :- where(Lesson, Room), whens(Lesson, T).

conflict(X,Y) :- (enrollment(X,LessonX), where(LessonX, Room), enrollment(Y,LessonY), where(LessonY, Room) ;
                  enrollment(X,LessonX), whens(LessonX, Time), enrollment(Y,LessonY), whens(LessonY, Time)),X\=Y.

meet(X,Y) :- enrollment(X, Lesson), enrollment(Y, Lesson), X\=Y.
