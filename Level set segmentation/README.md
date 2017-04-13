lab_4
----

**Задача:** Сегментация изображений – Level set метод

**Описание реализации:** Пользователь выбирает область, в которой точно присутствует объект. Результатом программы 
будет изображение с выделенным объектом.

**Время:**

| Изображение        | Последовательно | OpenMP | TBB | MPI+OpenMP |
| ------------------ |:---------------:|:------:|:---:|:----------:|
| Circle (600x600)   | 0.019093        |        |     |            |
| Square (444x444)   | 0.014493        |        |     |            |
| Ellipse (1301x800) | 0.033308        |        |     |            |

### Изображения

#### Circle (600x600)

![circle](images/circle.jpg "Source")
![circle](images/circle%20result.jpg "Result")

#### Square (444x444)

![square](images/square.jpg "Source")
![square](images/square%20result.jpg "Result")

#### Ellipse (1301x800)

![ellipse](images/ellipse.jpg "Source")
![ellipse](images/ellipse%20result.jpg "Result")
