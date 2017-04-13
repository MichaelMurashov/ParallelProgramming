lab_4
----

**Задача:** Сегментация изображений – Level set метод

**Описание реализации:** Пользователь выбирает область, в которой точно присутствует объект. Результатом программы 
будет изображение с выделенным объектом.

**Время:**

| Изображение | Последовательно | OpenMP | TBB | MPI+OpneMP |
| ----------- |:---------------:|:------:|:---:|:----------:|
| Circle      | 0.013016        |        |     |            |
| Square      | 0.00932         |        |     |            |
| Ellipse     | 0.029837        |        |     |            |

**Изображения:**

#### Circle 600x600

![circle](images/circle.jpg "Source")
![circle](images/circle%20result.jpg "Result")

#### Square 444x444

![square](images/square.jpg "Source")
![square](images/square%20result.jpg "Result")

#### Ellipse 1301x800

![ellipse](images/ellipse.jpg "Source")
![ellipse](images/ellipse%20result.jpg "Result")
