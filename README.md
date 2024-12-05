# TaskScheduler


## Постановка задачи

Разработать класс **`TTaskScheduler`**, который решает задачу выполнения связанных между собой задач. Это полезно для реализации графов вычислений, где:

- Узел графа представляет задачу.
- Ребро графа задает зависимость между результатами выполнения одной задачи и входными параметрами для другой.

Пример: нахождение корней квадратного уравнения (предполагается, что коэффициенты гарантируют два корня). Обычный подход потребует дважды вычислять корень из дискриминанта. Шедулер позволяет оптимизировать эту задачу, вычисляя дискриминант только один раз.

## Требования к классу `TTaskScheduler`

Класс должен предоставлять следующий интерфейс:

- **`add`**  
  Принимает задачу (вызываемый объект) и её аргументы. Возвращает идентификатор задачи
  
- **`getFutureResult`**  
  Позволяет получить результат задачи при выполнении

- **`getResult`**  
  Возвращает результат выполнения задачи. Если задача еще не выполнена, выполняет её

- **`executeAll`**  
  Выполняет все добавленные задачи с учетом их зависимостей

---

### Пример использования

```cpp
float a = 1;
float b = -2;
float c = 0;

TTaskScheduler scheduler;

auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));
auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));
auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<float>(id5) << std::endl;
std::cout << "x2 = " << scheduler.getResult<float>(id6) << std::endl;
```

---

### Ограничения

1. Запрещено использовать стандартную библиотеку, кроме контейнеров и умных указателей
2. Каждая задача принимает не более двух аргументов
3. Аргументы и возвращаемые значения задач должны быть `CopyConstructible`

---

### Тесты

- Для проверки функциональности используется **Google Test**

---