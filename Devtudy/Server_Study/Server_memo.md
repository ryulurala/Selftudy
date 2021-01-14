# MMO C# Server

## 개념 정리(시작하기 앞서)

### 동기 vs 비동기

|                                    | 동기 | 비동기 |
| :--------------------------------: | :--: | :----: |
|     요청과 결과가 동시에 발생      |  O   |   X    |
| 함수의 결과가 호출한 함수에서 처리 |  O   |   X    |

### Blocking vs Non-blocking

- Blocking  
  : 내가 일을 하고 있을 때, 내가 제어권을 갖는다.

- Non-blocking  
  : 내가 일을 하고 있을 때, 날 호출한 쪽이 제어권을 갖는다.

### Call-back(콜백 함수)

- 어떤 이벤트가 발생했을 때, 수행해야 하는 함수

#### Example 1: delegate(대리자) 이용

- `=`, `+=`, `-=` 로 참조 가능
  - `=`: `new` 대신 사용
  - `+=` / `-=`: delegate chain `등록` / `취소`

```C#
using System;

namespace DelegateExample
{
    class Program
    {
        delegate void MyDelegate();   // 대리자 정의

        static void Main(string[] args)
        {

            MyDelegate myDelegate;    // 대리자 변수 선언

            myDelegate = A;     // A 함수 등록, 처음에만 '=' 연산자 오버로딩 or new 키워드
            myDelegate += B;    // B 함수 등록
            myDelegate += delegate ()
            {
                Console.WriteLine("Anonymous()");   // 익명 함수 등록
            };
            myDelegate += () =>     // 람다 함수 등록
            {
                Console.WriteLine("Lambda()");
            };

            myDelegate();   // A() -> B() -> Anonymous() -> Lambda() 출력

            // A(), B(), 익명 함수, 람다 함수 모두 콜백 함수로 등록했다.(콜백 체인)
        }

        static void A()
        {
            Console.WriteLine("A()");
        }

        static void B()
        {
            Console.WriteLine("B()");
        }
    }
}
```

#### Example 2: event 이용

- 객체 상태 변화, 이벤트 발생 용도로 사용

- event 는 외부에서 직접 사용이 불가능하다.  
  (static 키워드를 사용하지 않는다면)

- 이벤트 만들기(과정)
  1. delegate 한정자로 델리게이트 선언(클래스 안밖 상관없음)
  2. 델리게이트의 인스턴스를 event 한정자로 선언
  3. 이벤트 핸들러 작성(델리게이트와 일치하는 메소드)
  4. 이벤트 핸들러에 등록(`+=` / `-=`으로 `등록` / `취소` 가능)

```C#
using System;

namespace DelegateExample
{
    class Program
    {
        delegate void MyDelegate();   // 대리자 정의
        static event MyDelegate MyEvent;    // 이벤트 선언

        static void Main(string[] args)
        {
            MyEvent = A;     // A 함수 등록, 처음에만 '=' 연산자 오버로딩
            MyEvent += B;    // B 함수 등록
            MyEvent += delegate ()
            {
                Console.WriteLine("Anonymous()");   // 익명 함수 등록
            };
            MyEvent += () =>     // 람다 함수 등록
            {
                Console.WriteLine("Lambda()");
            };

            MyEvent();   // A() -> B() -> Anonymous() -> Lambda() 출력

            // A(), B(), 익명 함수, 람다 함수 모두 콜백 함수로 등록했다.(콜백 체인)
        }

        static void A()
        {
            Console.WriteLine("A()");
        }

        static void B()
        {
            Console.WriteLine("B()");
        }
    }
}
```

#### Exmaple 3: Action 이용

- Action\<T, T, T, ...>  
  : Action 변수 명: 매개 변수 없음
  : Action\<T1>: 매개 변수 T1  
  : Action\<T1, T2>: 매개 변수 T1, T2  
  : Action\<T1, T2, T3>: 매개 변수 T1, T2, T3

- 무조건 반환 타입은 void 형이다.

- 반환 형이 없이 확실히 없을 때 사용.

```C#
using System;

namespace ActionExample
{
    class Program
    {
        static void Main(string[] args)
        {
            Action myAction;

            myAction = A;     // A 함수 등록, 처음에만 '=' 연산자 오버로딩 or new 키워드
            myAction += B;    // B 함수 등록
            myAction += delegate ()
            {
                Console.WriteLine("Anonymous()");   // 익명 함수 등록
            };
            myAction += () =>     // 람다 함수 등록
            {
                Console.WriteLine("Lambda()");
            };

            myAction();   // A() -> B() -> Anonymous() -> Lambda() 출력

            // A(), B(), 익명 함수, 람다 함수 모두 콜백 함수로 등록했다.(콜백 체인)
        }

        static void A()
        {
            Console.WriteLine("A()");
        }

        static void B()
        {
            Console.WriteLine("B()");
        }
    }
}
```

#### Example 4: Func 이용

- Func\<T, T, T, ..., return type>  
  : Func\<T1>: return T1, 매개 변수 없음  
  : Func\<T1, T2>: 매개 변수 T1 / return T2  
  : Func\<T1, T2, T3>: 매개 변수 T1, T2 / return T3  
  : Func\<T1, T2, T3, T4>: 매개 변수 T1, T2, T3 / return T4

- 반환 타입이 존재해야 함.

- 반환 형이 있고 반환 타입을 알고 있을 때 사용.

```C#
using System;

namespace FuncExample
{
    class Program
    {
        static void Main(string[] args)
        {
            Func<int> myFunc;     // 반환 타입이 있어야 함, <return type>

            myFunc = A;     // A 함수 등록, 처음에만 '=' 연산자 오버로딩 or new 키워드
            myFunc += B;    // B 함수 등록
            myFunc += delegate ()
            {
                Console.WriteLine("Anonymous()");   // 익명 함수 등록
                return 1;       // int 반환
            };
            myFunc += () =>     // 람다 함수 등록
            {
                Console.WriteLine("Lambda()");
                return 1;       // int 반환
            };

            myFunc();   // A() -> B() -> Anonymous() -> Lambda() 출력

            // A(), B(), 익명 함수, 람다 함수 모두 콜백 함수로 등록했다.(콜백 체인)
        }

        static int A()
        {
            Console.WriteLine("A()");
            return 1;       // int 반환
        }

        static int B()
        {
            Console.WriteLine("B()");
            return 1;       // int 반환
        }
    }
}
```

#### 콜백 함수(C# 문법) 정리

|                         |  delegate   |            event            |        Action        |         Func\<T>          |
| :---------------------: | :---------: | :-------------------------: | :------------------: | :-----------------------: |
| delegate 함수 형식 지정 |    필수!    |       delegate에 한정       |          X           |             X             |
|   반환 타입 명시 여부   |    가능     |       delegate에 한정       |  불가능(Only. void)  | 가능(<T, T, return type>) |
|          용도           | type 불명확 | 객체 상태 변화 or 사건 발생 | 반환 type 없음(명확) |   반환 type 있음(명확)    |

---

[서버 OT]

\*서버 : 다른 컴퓨터에서 연결이 가능하도록 대기 상태로 상시 실행 중인 프로그램 (ex 영업 중인 식당)

\*게임 서버의 종류

1. Web Server(aka. HTTP Server)

- 음식을 받고, 이후론 연락 끊김. 질의/응답 형태. (ex 테이크아웃 포장 전문 식당)
- 드물게 정보 요청/갱신.
- 먼저 접근할 일이 없다. (ex 물 따라드릴까요? 등)
- 실시간 Interaction X
- ASP.NET(C#), Spring(Java), NodeJS(JavaScript), Django, Flask(Python), PHP, ...
- Stateless

2. Game Server(aka. TCP Server, Binary Server, Stateful Server ...)

- 실시간 Interaction이 있다. (ex 일반 식당)
- 요청/갱신 횟수가 많다.
- 실시간 Interaction이 필요.
- 언제라도 직원이 손님한테 접근 가능.
- Stateful
- 게임/장르에 따라 요구 사항이 다르다.
- 최적의 프레임워크라는 것이 존재하기 힘듬.

\*식당 = 게임 서버 비유

- 손님 한도 = 최대 동시 접속자
- 손님의 일행 한도(인테리어) = 게임 장르 및 채널링
- 직원 역할 = 게임 로직(요리사), 네트워크(서빙), DB(결제)
- 직원 수 = 쓰레드 개수
- 요리사/서빙/결제 직원 비율 = 쓰레드 모델
- 주문 받는 방법 = 네트워크 모델
- 손님이 기다리는 시간 한도 = 반응성(FPS, MMORPG, ...)
- 장부 및 결제 = DB

---

[환경 설정]

- VS code에서 최상위 폴더를 만들고 Project 하위 폴더로 Server, ServerCore, DummyClient를 만든다.
- 각각 Project이므로 각 위치에서 "dotnet new console"로 프로젝트를 기반을 다진다.
- DummyClient는 Client역할을 한다.(500~1000명으로 인식되도록 패킷을 전달해준다)
- Server는 실질적인 서버
- ServerCore는 Server가 사용하는 핵심 기능들을 넣어 놓는다.
- "dotnet new sln"을 통해서 최상위 폴더에 솔루션 파일을 만든다
- "dotnet sln add \*"로 해당 폴더부터 하위 폴더 모두를 검사하여 .csproj를 담는다.
- "dotnet run -p (프로젝트 이름)" sln 파일 위치에서 실행 or 해당 디렉토리에서 "dotnet run" 으로 실행

---

[멀티쓰레드 개론]

- 코어가 1개일 때, 빠른 시간으로 짧게 전환하여 프로그램을 실행한다.
- 스케줄링으로 코어가 우선순위를 정하여 중요한 프로그램은 실행 시간을 보장한다.
- Heap 영역(new), Data 영역(static 변수)는 공유하지만 Stack은 각자만의 것이 있다.
- 멀티쓰레드 환경은 직원을 많이 두어 효율적으로 굴러가게 해야 한다.

---

[쓰레드 생성]

- C + W + Tab으로 빠른 콘솔 로그를 만들 수 있다.

- Main 함수가 사장이면 Thread는 직원을 고용하는 것이다.
- Main 함수가 죽으면 쓰레드도 죽는다.

* Thread 생성(= 정직원 고용) --- 길게 일을 할 때 쓴다.

1. System.Threading; : 쓰레드를 사용하기 위한 using
2. static void MainThread(Object state) : 함수를 정의하여 일을 정해준다.(static 전역으로 생성)
3. Thread thread = new Thread(MainThread) : MainThread 일을 하는 thread를 생성한다.
4. thread.Start(); : 쓰레드를 시작한다.

- thread.name = "..." : 쓰레드의 이름 지정
- thread.IsBackground = true; : C#은 Default로 ForeGround에서 돌지만(false이지만) BackGround로 돌게 한다.
- thread.join(); : BackGround 쓰레드가 끝날 때까지 기다린다.(C++도 같은 함수다)

* ThreadPool로 Thread 생성(= 단기 알바 고용) --- 짧게 일을 할 때 쓴다.

1. ThreadPool.QueueUserWorkItem(MainThread); : MainThread 함수의 일을 하는 Queue를 생성한다.(= 인력사무소)

- 인원 수를 설정 가능하지만 각자 무슨 일을 하는 것에 대한 것은 불가능하다.
- 재사용하여 효율적이다.
- ThreadPool.SetMinThreads(N, N); : 최소 인원을 정하는 함수, (인원 수, IO와 관련된 NetWork)
- ThreadPool.SetMaxThreads(N, N); : 최대 인원을 정하는 함수, 최대를 넘으면 먹통이 되는 단점이 있다.
- 먹통되는 단점을 Task로 해결 가능하다.
- ThreadPool.QueueUserWorkItem(obj => { while (true) { } }); // object를 받아서 => ...일을 해라.

* Task 생성(WorkThreadPool 단점 해결)

1. System.Threading.Tasks; : Task를 사용하기 위한 using
2. Task task = new Task(() => { while (true) { } }, TaskCreationOptions.LongRunning);
3. task.Start();

- ThreadPool에 포함되지만 옵션을 넣어서 별도 쓰레드로 생성하게 한다.
- 옵션이 없으면 먹통 상황은 같다.
- TaskCreationOptions.LongRunning : 오래 걸린다는 것을 알려주는 옵션.

---

[컴파일러 최적화]

- Thread.Sleep(N); : N ms = 0.001N s를 Sleep한다.
- task.Wait(); : Thread의 Join()과 같다.(Background task가 아니어도 기다린다)

* 컴파일러의 코드 최적화

- Debug mode와 Release mode에서는 다르게 프로그램이 실행한다.
  ex) Debug mode : while(bool){ ...; }
  Release mode : if(bool){ while(true) { ...; }}
- 코드를 작성한 것이 while문 안에 bool형의 조건을 건드는 것이 없을 때 발생한다.

* 코드 최적화 방지

- 간단한 방법 : static bool 변수 명; -> volatile static bool 변수 명;
- volatile은 휘발성 데이터로 코드 작성한 그대로 쓰게 한다.
- volatile은 C++에서는 조금 다르다.
- volatile을 안쓰는 것을 권장한다.
- Lock, Memory Barrier 등으로도 할 수 있다.

---

[캐시 이론]

- CPU 코어에는 ALU(연산 장치) + 캐시 장치가 있다.
- 캐시 장치에 기록해놓았다가 Ram(Main memory)에게 전달한다.

* Locality

- Tempolal Locality : 시간 - 최근 접근한 것이 또 접근할 확률이 높다.
- Spacial Locality : 공간 - 접근한 것의 근처에 접근할 확률이 높다.

- Single Thread 환경에서는 문제가 없지만 Multi Thread 환경에서는 잘못 전달되면 바꾸기 힘들다.

- C# 에서의 2차원 배열 : int[,] arr = new int[N, N]; -> 접근 방법 : arr[n, n];

* 시간 재는 법(Tick 하나는 100 ns = 1/10,000,000 s)

1. long now = DateTime.Now.Ticks;
2. long end = DateTime.Now.Ticks;
3. end - now

---

[메모리 배리어]

- 컴파일러(S/W)말고도 H/W도 코드 최적화를 한다.(스케줄링 - 순서를 바꾼다)
- volatile로 컴파일러의 코드 최적화를 막지만 H/W는 못 막는다.

* 메모리 배리어 효과

1. 코드 재배치 억제
2. 가시성 : 실제 메모리에 Flush하는 느낌(동기화)

- 메모리 배리어 종류 : Full, Store, Load Memory Barrier가 있다.

1. Full Memory Barrier (ASM(어셈블리) : MFENCE, C# : Thread.MemoryBarrier) : Store/Load를 둘 다 막는다.
2. Store Memory Barrier (ASM : SFENCE) : Store만 막는다.
3. Load Memory Barrier (ASM : LFENCE) : Load만 막는다.

- 사용 방법

* Thread.MemoryBarrier();
* Store 후의 메모리 배리어, Load 전에 메모리 배리어를 사용한다.

* Task.WaitAll(Taks[]); : 기다릴 Task를 ','로 구분하여 파라미터에 넣는다.

---

[Interlocked]

- 경합 조건(Race Condition)과 Interlocked

* 원자성(Atomic) : 고유하고 유일하다
* 컴퓨터의 최적화로 인해 값을 복사하여 사용하기 때문에 Atomic하지 않다.
* 따라서 Atomic하게 만들어 같은 것을 사용하게 하고 경합(Race)하게 한다.
* 경합이라는 것은 순서가 보장돼 있다.
* _주의_ : 경합하고 공유하는 원자성이 있는 멋대로 변수는 읽어서 사용하면 안된다(if 절)
* Interlocked.Increment(ref 변수 명); : Atomic하게 변수를 1 증가시킨다(주소 값을 넘기며 반환은 증가한 int 값이다)
* Interlocked.Decrement(ref 변수 명); : Atomic하게 변수를 1 감소시킨다(변수의 값을 모르며 메모리 배리어를 간접적으로 사용한다)

---

[Lock 기초]

- Interlocked은 int형을 증가시키고 감소시키고 단순한 일만 가능하다.

* { Monitor.Enter(Object obj) ... Monitor.Exit(Object obj) }

- Interlocked보다 복잡한 일 가능.
- Enter와 Exit 사이에서는 싱글 쓰레드 환경처럼 Atomic하게 사용할 수 있다.
- 일종의 문을 잠구는 행위(obj는 자물쇠 역할)와 문을 여는 행위이다.
- 단점 : DeadLock 상태 발생
- DeadLock : Enter하고 Exit이 되지 않고 return; 이 되버리면 atomic한 변수를 다른 곳에서 사용할 때 무한정으로 기다리게 된다(먹통이 된다)
- 해결 :

1. try-finally로 finally는 Exception이 발생해도 한 번은 실행되는 것으로 해결한다.(코드가 복잡해진다)
2. lock(Object obj){ ... }을 사용한다.(대부분 이 방법 사용)

- Ex 문제 상황
  Monitor.Enter(obj);
  ...
  return;
  Monitor.Exit(obj); // return 되어 DeadLock 발생.

1. try-finally 해결
   try
   {
   Monitor.Enter(obj);
   ...
   return; // return 되어도 finally 함수를 한 번 실행하여 Exit해준다.
   }
   finally
   {
   Monitor.Exit(obj);
   }

2. _lock(Object obj) 해결_ 많이 쓴다.
   lock(obj)
   {
   ...
   return; // 내부적으로 Monitor함수다. return; 되기 전에 자동으로 Monitor.Exit()을 처리해준다.
   }

- 그래도 데드락은 *다른 조건(Object=자물쇠가 여러 개)*으로 발생한다.
- 데드락 발생하는 조건을 없애는 목적이다.

---

[데드락(DeadLock)]

- DeadLock(교착 상태) 상황

* 여러 자원을 각자 가지고 서로 가지려고 할 때 발생하는 경우가 많다.
  ex) A는 a를 가지고 있고 b를 가지려 할 동시에, B는 b를 가지고 있고 a를 가지려 할 때

- 해결 방법

* 처음부터 구조를 만들 때 데드락 상황이 일어나지 않도록 한다.
* 하지만 일반적인 경우,
  이미 Base는 다진 상태에서 유지 보수를 하게 된다면 Crash를 내어 데드락이 일어나는 상황을 보고 고치는 것이 현명하다(try-catch 방식)
* 사실 조금만 시간을 어긋나게 하면(Thread.Sleep(N); ) 발생하지 않을 수 있다.
* 주로 네트워크에서 일어나고 개발, QA 단계에서는 잘 발생하지 않고 출시하는 상황에서 많이 발생한다.

1. Monitor.TryEnter(Object obj, TimeSpan timeSapn); : 정해 놓은 시간만큼 시도하다 안되면 포기하게 한다.
2. ID를 부여하여 우선 순위를 부여하고 추적하여 데드락을 해결할 수도 있다.

---

[Lock 구현 이론]

- 무작정 기다리기. - 단점 : 안나오면 내 일을 아예 못함(SpinLock)
- 내 일을 하다가 나중에 다시 온다. - 단점 : 다시 돌아왔는데 다른 사람이 들어감(Context Switching)
- 관리자가 중재하기 - 단점 : 관리자에게 비용이 들어감(AutoResetEvent or ManualResetEvent, Mutex)

---

[SpinLock]

- SpinLock

* 무작정 기다리기
* 동시에 들어갈 수도 있어 비교 연산을 사용하여야 한다.(0이면 1, 1이면 0)

- ex )
  class SpinLock
  {
  volatile bool \_locked = false; // 자물쇠

  public void Acquire() // Monitor.Enter(); 와 비슷하다.
  {
  // while(\_locked){ // 잠금이 풀리기를 기다리는 행위}
  \_locked = true; // 이제 자물쇠는 자신의 것
  }

  public void Release() // Monitor.Exit(); 와 비슷하다.
  {
  // 자물쇠를 돌려줌
  \_locked = false;
  }
  }

* 동시에 들어갔기 때문에 버그가 발생한다.

* 해결(원자 단위의 연산 사용)

1. Exchange(ref T location, T value) 사용 : location에 value 값으로 설정, return 값은 본래의 값
   volatile int \_locked = 0; // Exchange를 사용하기 위해 Type 변경

public void Acquire()
{
while(true)
{
int original = Interlocked.Exchange(ref \_locked, 1); //\_locked = 1;과 같다.
if(original == 0) { break; }
}
}

2. CompareExchange(ref T location, T desire, T expected) 사용 : location이 expected 값이면 desire 값을 넣는다, return 값은 본래의 값
   volatile int \_locked = 0; // CompareExchange를 사용하기 위해 Type 변경

public void Acquire()
{
while(true)
{
int original = Interlocked.Exchange(ref \_locked, 1, 0); // if (\_locked == 0) { \_locked = 1; } 과 같다.
if(original == 0) { break; }
}
}

- CompareExchange는 CAS, Compare-And-Swap 구조이다.

- Release(){ // 사실 자물쇠만 돌려주면 할 일이 없다.}

---

[Context Switching]

- Context Switching

* 무작정 기다리지 않고 내 일을 하고 올게! 라는 의미
* 방법 3 가지 : 각각 장점과 단점이 있다.
  1. Thread.Sleep(N); // 무조건 휴식 -> 무조건 N ms 정도 쉰다.
  2. Thread.Sleep(0); // 조건부 양보 -> 나보다 우선순위 낮으면 양보 불가(다시 본인), 높거나 같으면 양보
  3. Thread.Yield(); // 관대한 양보 -> 실행 가능한 쓰레드에게 양보, 없으면 남은 시간 소진
* 순서

1. Register에 들어있는 State 값을 RAM에 저장
2. Switching
3. Register에 들어올 값 복원

- 비용이 비싸다

ex)
public void Acquire()
{
while(true)
{
// 기다리기
int original = Interlocked.Exchange(ref \_locked, 1, 0); // if (\_locked == 0) { \_locked = 1; } 과 같다.
if(original == 0) { break; }

    // 내 일을 하러 간다(쉬다 온다)
    Thread.Sleep(1);    // 무조건 휴식 -> 무조건 1ms 정도 쉰다.
    Thread.Sleep(0);    // 조건부 양보 -> 나보다 우선순위 낮으면 양보 불가(다시 본인), 높거나 같으면 양보
    Thread.Yield();     // 관대한 양보 -> 실행 가능한 쓰레드에게 양보, 없으면 남은 시간 소진

}
}

- Sleep()을 실행하면 CPU 점유권을 포기하고 커널 모드로 들어가 OS에게 실행 권한을 넘겨준다.
- OS가 그 다음 실행할 쓰레드를 결정 : 오래 기다린 순서 + 중요도(Initialized)
- 현재 실행 중인 모든 쓰레드에서 Sleep과 같은 대기를 요청한 상태가 아닌 이상,
  남는 쓰레드가 없을 경우는 거의 없다고 보면 된다(실제 프로세스 개수 생각)
- 실제로는 모든 프로세스는 CPU Time Slice를 간절하기 받기를 원하는 상태이다.

---

[AutoResetEvent]

- Auto Reset Event vs Manual Reset Event

* 관리자가 중재(관리자 = Kernel)
* 자동 vs 수동(쓰레드를 받아들일 문을 닫는 행위)
* 일종의 Kernel단에서의 Flag(bool) 개념이다.
* 다른 이유가 없다면 Auto가 더 깔끔하다(Manual은 두 차례 나눠서 하기 때문에 버그가 발생할 수 있다)
  ex)

1. AutoResetEvent
   AutoResetEvent \_available = new AutoResetEvent(bool initialState); // true : available, false : unavailable

public void Acquire() // Monitor.Enter();
{
\_available.WaitOne(); // 입장 시도 : AutoResetEvent면 입장 후 바로 Auto로 닫아준다.
}
public void Release() // Monitor.Exit();
{
\_available.Set(); // 문을 열어주는 행위. flag = true;
}

2. ManualResetEvent
   ManualResetEvent \_available = new ManualResetEvent(bool initialState);

public void Acquire() // Monitor.Enter();
{
\_available.WaitOne(); // 입장 시도
\_available.Reset(); // 문을 닫아주는 행위. flag(bool) = false; // Auto랑 차이
}
public void Release() // Monitor.Exit();
{
\_available.Set(); // 문을 열어주는 행위. flag(bool) = true;
}

- Mutex

* Kernel 동기화 객체
* ResetEvent 보다 많은 State를 담고 있다. : Thread ID, Lock count 등
* ResetEvent 보다 조금 더 느리다.
* 클래스를 만들 필요 없이 Mutex 객체를 이용한다.
  ex)
  static Mutex \_lock = new Mutex();
  {
  \_lock.WaitOne();
  ...
  \_lock.ReleaseMutex();
  }

---

[ReaderWriterLock]

- Lock 종류 3가지

1. 근성 : \_lock{ ... } : 내부에서는 Monitor로 되어 있다.
2. 양보 : Sleep(0), Sleep(N), Yield() 이용
3. 갑질 : 프로그램-프로그램단에서는 유리할 수 있다.

- 게임에 따라서 멀티쓰레드를 사용할 지 구분한다.
- 멀티쓰레드 환경은 난이도가 많이 높아진다.

* SpinLock 예시 : 사실 양보까지도 한다.
  ex) 구현돼 있는 SpinLock API
  static SpinLock \_spinLock = new SpinLock();

  bool lockTaken = false;
  try
  {
  \_spinLock.Enter(ref lockTaken);
  }
  finally
  {
  if(lockTaken){ \_spinLock.Exit(); } // Exception이 났을 경우
  }

* ReaderWriterLock

- 최종적으로는 DB에 저장되겠지만 DB부하와 다른 이유로 DB에서 긁어와 서버 메모리에서 처리해야 할 때가 있다.
- 특정 던전을 유도하기 위해 특정 시간에 운영자가 운영툴로 게임 보상을 추가해야한다면
  보상이 5개에서 6개로 오를 경우, Lock을 걸지 않고는 Data에 수정하기는 어렵다.
  이럴 경우, User들은 Read할 경우는 여러 번이지만 Write는 한 번이다.
  여기서 Lock을 같은 Writer의 같은 수준을 걸게 되면 낭비된다.
- ReaderWriterLock or ReaderWriterLockSlim 객체
  : Read는 여러 번, Write는 한 번 정도의 Lock 수준을 걸고 싶을 때 사용한다.
- ReaderWriterLockSlim이 최신이다(더 많이 씀)

ex)
static ReaderWriterLockSlim \_rwLock = new ReaderWriterLockSlim();

static Reward GetRewardById(int \_id)
{
\_rwLock.EnterReadLock();
// ... User(여러 번)
\_rwLock.ExitReadLock();
return null;
}
static void AddReward(int \_id)
{
\_rwLock.EnterWriteLock();
// ... VIP(한 번만)
\_rwLock.ExitWriteLock();
}

---

[ReaderWriterLock 구현 연습]

- ReaderWriterLock 구현

1. 정책 : 재귀적 락을 허용할 지(Yes or No), 스핀락 정책(5000번 -> Yield)

- 재귀적 락(Yes): WriteLock->WriteLock(Ok), WriteLock->ReadLock(Ok), ReadLock->WriteLock(No)
- - const int MAX_SPIN_COUNT = 5000;

2. Bit Masking을 이용하여 원자성을 강조한다.

- const int EMPTY_FLAG = 0x00000000;
- const int WRITE_MASK = 0x7FFF0000;
- const int READ_MASK = 0x0000FFFF;
- int \_flag = EMPTY_FLAG;
  int형 32bits:
  [Unused(1)], : 음수가 되는 것을 방지
  [WriteThreadId(15)], : Write 한 번에 한 쓰레드만 잡을 때, 그 쓰레드 ID : 상호-배타적임
  [ReadCount(16)] : 여러 쓰레드들이 동시에 잡을 때, 쓰레드 개수

3.  if) 재귀적 허용(Yes): int \_writeCount = 0; // 상호-배타적이기 때문에 별도의 변수 가능

4.  함수 구현(4개)
    // Write Lock을 건다(여러 개가 동시에 들어와도 혼자만 가능)
    public void WriteLock()
    {
    // if) 재귀적 허용(Yes): 동일 쓰레드가 WriteLock을 이미 획득하고 있는지 확인
    int lockThredId = (\_flag & WRITE_MASK) >> 16;
    if(Thread.CurrentThread.ManagedThreadId == lockThreadId)
    {
    // 동일 쓰레드가 획득 성공
    \_writeCount++;
    return;
    }

    // 아무도 WriteLock or ReadLock을 획득하고 있지 않을 때, 경합하여 소유권을 얻는다.
    int desired = (Thread.CurrentThread.ManagedThreadId << 16) & WRITE_MASK;
    while(ture)
    {
    for(int i=0; i<MAX_SPIN_COUNT; i++)
    {
    // 시도를 해서 성공하면 return;

          // 잘못된 코드 예시
          if(_flag == EMPTY_FLAG)   // 1단계
          {
            _flag = desired;        // 2단계
            return;                 // 1단계를 동시에 통과하여 2단계를 할 수도 있으므로 버그가 생긴다.
          }

          // 올바른 코드
          if(Interlocked.CompareExchange(ref _flag, desired, EMPTY_FLAG) == EMPTY_FLAG)
          {
            _writeCount = 1;    // if) 재귀적 락(Yes)
            return;
          }
        }

    }

    // WriteLock을 실패했을 경우 양보한다.(1개만 (한 번 or 여러 번) 성공할 수 있기에)
    Thread.Yield();
    }

// Write Lock을 가지고 있는 쓰레드가 UnLock 한다.
public void WriteUnLock()
{
int lockCount = --\_writeCount; // if) 재귀적 락(Yes)
if(lockCount == 0) // 한 개의 쓰레드가 여러 번 했을 경우 때문에
{
// WriteLock을 하고 있는 쓰레드만 WriteLock을 해제할 수 있다.
Interlocked.Exchange(ref \_flag, EMPTY_FLAG); // 초기 상태로 바꿔준다(UnLock한다)
}
}

// Read Lock을 한다. 여러 쓰레드가 동시에 들어와서 사용 가능
public void ReadLock()
{
// 동일 쓰레드가 WriteLock을 이미 획득하고 있는지 확인
int \_lockThreadId = (\_flag & WRITE_MASK) >> 16;
if(Thread.CurrentThread.ManagedThreadId == \_lockThreadId)
{
// 동일 쓰레드가 획득을 성공
Interlocked.Increment(ref \_flag);
return;
}

    // 아무도 WriteLock을 획득하고 있지 않으면, ReadCount를 1 증가시킨다.
    while(true)
    {
      for(int i=0; i<MAX_SPIN_COUNT; i++)
      {
        // 아무도 WriteLock을 획득하고 있지 않다.

        // 잘못된 예(in 멀티 쓰레드 환경)
        if((_flag & WRITE_MASK) == EMPTY_FLAG)  // 1단계
        {
          _flag = _flag + 1;                    // 2단계(Read한 쓰레드 개수 증가)
          return;                               // 1단계를 동시에 통과하여 1증가 버그 발생
        }

        // 올바른 예(in 멀티 쓰레드 환경) ****중요****
        int expected = (_flag & READ_MASK)  // WriteFlag를 0으로 Mask, if: A(0), B(0) 동시에 하고 다음에
        // A(0->1), B(0->1) 둘 중에 먼저 한 쓰레드만 통과 가능(나중은 다음 for문 턴에 가능)
        if(Interlocked.CompareExchange(ref _flag, expected+1, expected) == expected)
        {
          return;
        }
      }

      // 양보
      Thread.Yield();
    }

}

// Read UnLock을 한다. 여러 쓰레드가 동시에 들어와서 사용 가능
public void ReadUnLock()
{
// UnLock한 애들 개수 1 감소
Interlocked.Decrement(ref \_flag);
}

---

[Thread Local Storage]

- 멀티 쓰레드 환경이 다 좋은 것만은 아니다.(비용이 있어서)

* TLS(Thread Local Storage)

- 쓰레드의 고유한 전역 변수: 다른 쓰레드에게는 영향을 주지 않는다.
- 쓰레드 하나가 일감을 모아서 사용하는 바구니 느낌이다.

ex 1) TLS(No)
static string ThreadName; // 일반적인 전역 변수
static void WhoAmI()
{
ThreadName = $"My Name is {ThreadName.CurrentThread.ManagedThreadId}"; // ThreadName 할당
Thread.Sleep(1000); // 혹시나 다른 쓰레드가 바꿔도 바뀌는지
System.Console.WriteLine(ThreadName);
}

- ThreadName을 Thread들이 모두 공유해서 사용한다.
- 한 쓰레드가 이름을 출력할 때 ThreadName을 바꿔서 겹치는 버그가 발생한다

ex 2) TLS(Yes)
static ThreadLocal<string> ThreadName = new ThreadLocal<string>();
static void WhoAmI()
{
// 자신의 영역의 이름을 바꾼다
ThreadName.Value = $"My Name is {Thread.CurrentThread.ManagedThreadId}";
Thread.Sleep(1000); // 혹시나 다른 쓰레드가 바꿔도 바뀌는지
System.Console.WriteLine(ThreadName.Value);
}

- ThreadName을 각 Thread들의 고유한 값으로 가지고 있다.(전역 변수여도)
- 하지만 ThreadPool의 Min과 Max의 값을 고정시키면 Thread Pooling으로 인해 동일한 쓰레드가 Repeat하는 경우가 생긴다.
  ex 3) TLS(Yes), repeat환경 테스트
  static ThreadLocal<string> ThreadName = new ThreadLocal<string>(()=>{
  return $"My name is {Thread.CurrentThread.ManagedThreadId}";
  });
  static void WhoAmI()
  {
  bool repeat = ThreadName.IsValueCreated; // true: 이미 한 번 만들었다, false: 만든적 없다
  if(repeat){ // 반복됨 ture: 이미 만들었었다. }
  else{ // 한 번도 만들지 않았다, value가 null일 때, 람다 실행}
  }

- Parallel.Invoke(WhoAmI, WhoAmI, WhoAmI, WhoAmI, WhoAmI, WhoAmI, WhoAmI); :
  매개 변수 넣는 만큼 ThreadPool에서 Task를 꺼내서 사용한다.

- ThreadName.Dispose(); : 람다식을 만든 것을 날린다.

---

[네트워크 기초 이론]

- 통신 구조 vs 택배 구조

* 라우터 = 택배 배송업체
* 네트워크 = 아파트 단지(현대 아파트 or 삼성 아파트)
* 스위치 = 단지 내 경비실
* 컴퓨터 = 집
* 사설 주소 = 김씨 집, 최씨 집
* 공식 주소(IP) = 201호, 502호

---

[통신 모델]

- 통신 모델(TCP/IP-5Layer) vs 택배 모델

* 어플리케이션(UI: HTTP, FTP, DNS) = 상품
* 트랜스포트(전송 확인/오류 해결: TCP(MMO), UDP(FPS)) = 배송 정책
* 네트워크(네트워크 간 경로 설정: IPv4, IPv6, 장치: 라우터) = 집 최종 주소: 일정하다
* 데이터 링크(네트워크 내 경로 설정: 이더넷, PPP, 장치: 스위치) = 단지 경로
* 피지컬(신호 처리, 케이블/허브) = 택배 운송

- OSI-7Layer Model(before TCP/IP-5Layer)

* 어플리케이션(7) ---- 주소: 도메인(Domain)
* 프레젠테이션(6)
* 세션(5)
* 트랜스포트(4)
* 네트워크(3) ---- 주소: IP, 장치: 라우터
* 데이터링크(2) ---- 주소: MAC(기기에 따라 고정), 장치: 스위치
* 피지컬(1)

* 단계가 증가될 때마다 정보가 추가되어 합쳐서 보낸다.

---

[소켓 프로그래밍 입문 #1]

- 식당(식당/손님) vs 소켓 구조(서버/클라이언트)

* 식당 입장:

  1. 문지기 고용 = Listener 소켓 준비
  2. 문지기 교육: 주소, 입구 = Bind(서버 주소 + Port를 소켓에 연동)
  3. 영업 시작 = Listen
  4. 안내 or 거절: 손님 연락 받고 대리인 입장 or 거절 = Accept
  5. 손님과 대리인 통화 = 클라이언트 대리인(Session)을 통해 송수신 가능

* 손님 입장:
  1. 휴대폰 준비 = 소켓 준비
  2. 식당 번호로 입장 문의 = 서버 주소로 Connect
  3. 안내 받은 대리인과의 통화 = 소켓을 통해 대리인(Session)소켓과 패킷 송수신 가능

---

[소켓 프로그래밍 입문 #2]

- 주소 사전 처리(Server, Client 동일)

* 최종 주소를 알아내기 위한 DNS(Domain Name System) 사용
  string host = Dns.GetHostName(); // HostName을 가져온다. ex) SRChoi
  IPHostEntry ipHost = Dns.GetHostEntry(host); // ex) System.Net.IPHostEntry
  IPAddress ipAddr = ipHost.AddressList[0]; // IP주소는 분산 서버에 따라 여러 개 출력 가능, ex) IP 주소
  IPEndPoint endPoint = new IPEndPoint(ipAddr, 7777); // 최종 주소, Port는 입구 위치(정문, 후문) ex) IP 주소 + Port 번호

\***\*\*\*\*\*** Server Socket 단계 \***\*\*\*\*\***

1. 소켓 생성: 문지기를 만든다.
   // AddressFamily: IPv4 or IPv6(네트워크 망에 내장된 DNS서버에 의해 설정돼있다)
   // SocketType.Stream, ProtocolType.Tcp: TCP(Stream을 쓰고 프로토콜은 TCP를 쓴다)

- Socket listenSocket = Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)

2. 소켓 연동: 문지기를 교육시켜 문의한 사람들만 안내한다.
   // EndPoint = IP + Port

- listenSocket.Bind(EndPoint endPoint);

3. 소켓 열기: 입구를 연다.
   // backlog: 최대 대기 수 Listen --- 동시 접근 발생 때문에

- listenSocket.Listen(int backlog);

4. 접속 허용: 사람을 안내한다.
   // Accept()의 return: Session(대리인)의 Socket = 안내받은 대리인

- Socket clientSocket = listenSocket.Accept();
- Blocking 함수다. = 대리인이 입장을 안하면 무한정 대기한다.

  5.0) 받을 데이터 크기 지정(사전 처리)

- byte[] recvBuffer = new Byte[N]; // Data Buffer

  5.1) 데이터 수신: Client로부터 데이터를 받는다.
  // Receive의 return: 받은 Byte 수

- int recvBytes = ClientSocket.Receive(byte[] buffer);

  5.2) 받은 데이터 디코딩
  // 문자 규약: Data Buffer, 읽을 시작 인덱스, Byte 수

- string recvData = Encoding.UTF8.GetString(byte[] bytes, int index, int count);

  6.0) 전송할 데이터 인코딩(사전 처리)

- byte[] sendBuff = Encoding.UTF8.GetBytes(String string);

  6.1) 데이터 송신: Client에게 데이터를 전송한다.
  // 인코딩된 데이터 전송

- clientSocket.Send(byte[] buffer);

7. 클라이언트 연결 끊기

- clientSocket.Shutdown(SocketShutdown.Both); // 예고(옵션): 닫을거다, 듣기도 싫고 말하기도 싫다.
- clientSocket.Close();

- 혹시나 예외가 발생할 수도 있으니 소켓 2단계부터 try-catch()로 처리해준다.

\***\*\*\*\*\*** Client Socket 단계 \***\*\*\*\*\***

1. 소켓 생성(주소 사전 처리 후): 대화할 휴대폰 설정

- Socket socket = new Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)

2. 접속 요청: 문지기한테 입장 문의
   // 서버 주소(Endpoint = IP + Port)로 연결 요청

- socket.Connect(EndPoint endPoint);
- Blocking 함수로 무한정 대기한다(게임에서 치명적)

  3.0) 전송할 데이터 크기와 인코딩(사전 처리)

- byte[] sendBuff = Encoding.UTF8.GetBytes(String string);

  3.1) 데이터 송신: Server session(대리인)에게 데이터를 전송한다.
  // SendBuff에 있는 것을 한 번에 보냄

- int sendBytes = socket.Send(sendBuff);
- Blocking 함수로 Server session이 받질 못하면 무한정 대기한다(게임에서 치명적)

  4.0) 받을 데이터 크기 지정

- byte[] recvBuff = new Byte[N];

  4.1) 데이터 수신: Server session(대리인)으로부터 데이터를 받는다.

- int recvBytes = socket.Receive(byte[] buffer);
- Blocking 함수로 Server session으로부터 받질 못하면 무한정 대기한다(게임에서 치명적)

  4.2) 받은 데이터 디코딩

- string recvData = Encoding.UTF8.GetString(byte[] bytes, int index, int count);

5. 연결 끊기: Server session(대리인)와의 연결을 끊는다.

- socket.Shutdown(SocketShutdown.Both); // 예고(옵션)
- socket.Close();

- 혹시나 예외가 발생할 수도 있으니 소켓 2단계부터 try-catch()로 처리해준다.

---

[Listener]

- Listener 클래스를 만들어 Refectoring한다.

* 비동기 방식은 두 단계로 나누어야 한다(RegisterAccept(등록), AcceptCompleted(완료))

ex) 비동기 Listener 방식
class Listener
{
Socket \_listenSocket; // ServerSocket
Action<Socket> \_onAcceptHandler // Event action

// 초기화 함수
public void init(IPEndPoint endPoint, Action<Socket> onAcceptHandler)
{
// TCP, Stream을 사용하는 ServerSocket
\_listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
\_onAcceptHandler += onAcceptHandler; // 매개 변수의 Event 등록
\_listenSocket.Bind(endPoint); // 소켓 연동
\_listenSocket.Listen(int backlog); // 소켓 열기: 동시 접속자일 때, 최대 대기수(backlog)

    // 이벤트 방식으로 비동기 구현(콜백으로 전달)
    SocketAsyncEventArgs args = new SocketAsyncEventArgs();       // 비동기 Event Arguments
    args.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);    // 이벤트 핸들러에 등록
    RegisterAccept(args);   // 초기화 단게에서 미리 등록(예약)

}

// Accept를 예약한다.
public RegisterAccept(SocketAsyncEventArgs args)
{
args.AcceptSocket = null; // 두 번째부터 기존에 들어있던 값을 초기화해야한다.
bool pending = \_listenSocket.AcceptAsync(args); // return: pending(보류) 여부, Async: 비동기=안되면 return먼저
if(pending == false)
{
// 클라이언트 접속 완료(보류가 false가 되었기 때문에)
OnAcceptCompleted(null, args);
}
// 접속 보류됨(클라이언트 접속 안함, 서버는 return 시켰음)
}
// EventHandler<TEventArgs>(Object sender, TEventArgs e)이기 때문에 매개 변수 일치 시킴
public OnAcceptCompleted(Object sender, SocketAsyncEventArgs args)
{ // 데드존
// Accept가 성공했는지 확인(SoccetError로 체크 가능)
if(args.SocketError == SocketError.Success) // 별도의 쓰레드로 실행해줌
{
// Accept 성공, 서버가 해줘야 할 일
// \_onAcceptHandler를 소유한 쓰레드가 args.AcceptSocket 처리
\_onAcceptHandler.Invoke(args.AcceptSocket);
}
else
{
// 에러 처리(성공 이외의)
}
RegisterAccept(args); // 다음 턴을 위해 등록(예약)
}
}

- OnAcceptHandler 함수에서 Recieve, Send, close 처리

---

[Session #1]

- 스택 오버플로우가 발생하지 않도록 AsycEventArg를 많이 둔다.(현실적으로 스택오버플로우 X)

* Session

- 보내고 받고 종료를 담당한다
- Receive
- Send
- Disconnect

ex) 비동기로 Receive() 구현: 비동기므로 두 단계(준비, 1.등록, 2.완료)로 구분된다.
class Session
{
Socket \_socket;
volatile int \_disconnected = 0;

public void start(Socket socket)
{
\_socket = socket; // ClientSocket
SocketAsyncEventArgs recvArgs = new SocketAsyncEventArgs(); // 비동기 이벤트
recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted); // 자동 콜백

    // new SocketAsyncEventArgs().UserToken : 식별자 or 연동하고 싶은 데이터를 받고 싶을 때 사용

    // SetBuffer(메모리, offset, count)
    // Session끼리 버퍼를 나눠서 쓸 때를 위해 offset과 count가 있다.
    recvArgs.SetBuffer(new byte[1024], 0, 1024);

    // 등록(예약)
    RegisterRecv(recvArgs);

}

void RegisterRecv(SocketAsyncEventArgs args)
{
// Non-blocking 버전의 Receive
bool pending = \_socket.ReceiveAsync(args); // 예약 확인하면서 Event를 호출하여 보낸다.
if(pending == false)
{
// 받는 것을 성공
OnRecvCompleted(null, args); // Send 이벤트가 얘를 호출한다.
}
}

void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
{
// 상대방이 연결을 끊으면 가끔 0 byte로 온다.
if(args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
{
// TODO(Connect: 데이터 받음)
try
{
// (버퍼, offset(어디부터 시작), 받은 바이트 수)
string recvData = Encoding.UTF8.GetString(args.Buffer, args.Offset, args.BytesTransferred);

        // Receive를 받을 준비를 다시 함
        RegisterRecv(args);
      }
      catch(Exception e)
      {
        // 에러 처리
      }
    }
    else
    {
      // TODO Disconnect
      Disconnect();
    }

}

public void Disconnect()
{
// Disconnect를 두 번하면 에러 난다.
// \_disconnected = 1, original value 1이면 또 close() 못하도록 함
if(Interlocked.Exchange(ref \_disconnected, 1) == 1)
{
return;
}

    // 쫓아낸다
    _socket.Shutdown(SocketShutdown.Both);
    _socket.close();

}
}

---

[Session #2]

- Send()함수의 비동기적 호출은 Receive의 비동기적 호출과는 다르다.
  이유는 Receive는 언제 올 지를 알지만 Send는 언제 할 지를 모르기 때문이다.

ex) 비동기로 Send() 구현: 비동기므로 두 단계(준비, 1.등록, 2.완료)로 구분된다.

- 필드 변수
  object \_lock = new Object(); // lock을 쓰기 위해서
  Queue<byte[]> \_sendQueue = new Queue<byte[]>(); // sendBuff를 넣고 한 번에 보내기 위해서
  bool \_pending; // 다른 쓰레드가 예약했는지 구분
  SocketAsyncEventArgs \_sendArgs = new SocketAsyncEventArgs(); // 재사용 하기 위해서

- 멤버 함수
  public void start(Socket socket)
  {
  // Receive용
  ...
  ...
  ...
  // Send용
  \_sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnSendCompleted); // 자동 콜백
  }

  public void Send(byte[] sendBuff) // 언제 할 지 예측 불가하다.
  {
  lock(\_lock) // 다른 쓰레드 들어오기 방지용
  {
  \_sendQueue.Enqueue(sendBuff); // Queue에만 넣고 스킵하는 쓰레드 발생 가능
  if(\_pending == false) // 첫 번째로 Send() 호출.(전송까지 가능하다)
  RegisterSend();
  }
  }

  void RegisterSend()
  {
  // 이미 앞에 Send()함수에서 lock을 걸어서 호출하기 때문에 락을 걸 필요 X
  \_pending = true;
  byte[] buff = \_sendQueue.Dequeue();
  \_sendArgs.SetBuffer(buff, 0 buff.Lenth);

  // SendAsync를 여러 번 호출하면 부하가 심하다.
  bool pending = \_socket.SendAsync(\_sendArgs); // 예약 확인 하면서 sendArgs의 이벤트가 발생하여 보낸다
  if(pending == false)
  {
  // 보내는 것이 성공
  OnSendCompleted(null, \_sendArgs);
  }
  }

  void OnSendCompleted(object sender, SocketAsyncEventArgs args)
  {
  lock(\_lock) // Start에서 들어오는 쓰레드가 있을 수도 있어서 락을 걸어준다.
  {
  // 상대방이 연결을 끊으면 가끔 0 byte로 오기 때문에
  if(args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
  {
  try
  {
  // 다른 쓰레드가 Enqueue했던 것을 처리해야한다.
  // 내가 예약하고 나서 pending = false; 하기 전에 다른 쓰레드가 예약을 했을 경우
  if(\_sendQueue.Count>0)
  {
  RegisterSend(); // 내가 그 쓰레드가 Enqueue한 것을 처리한다.
  }
  else
  {
  \_pending = false; // 최종적으로 false(예약 취소)
  }
  }
  catch(Exception e)
  {
  // 에러 출력
  }
  }
  else
  {
  // 상대방과 연결을 끊어짐
  Disconnect();
  }
  }
  }

- 다른 쓰레드가 와서 버퍼를 계속 쓰기 때문에(SetBuffer) 완벽하게 해결되지는 않았다.

---

[Session #3]

- Session send()함수 효율적으로 리펙토링
  ex)

* 필드 변수 수정
  // ArraySegment 사용하기 위해서, 다른 쓰레드가 예약했는지도 구분(Count로)
  List<ArraySegment<byte>> \_pendingList = new List<ArraySegment<byte>>();

* 멤버 함수
  void RegisterSend()
  {
  // ary[][][][][][][] 일 때, C++은 Pointer로 시작 주소를 넘길 수 있지만, C#은 시작주소와 offset을 넘겨줘야 한다.
  // 기본적으로 버퍼의 범위를 표현할 때는 {버퍼(array), 시작인덱스(offset), 크기(size)}를 넘겨준다.
  while(\_sendQueue.Count > 0)
  {
  byte[] buff = \_sendQueue.Dequeue();
  \_pendingList.Add(new ArraySegment<byte>(buff, 0, buff.Lenth)); // 더 효율적임(힙X, 스택O)
  // 기존(같이 쓰는 Buffer를 건드리므로)
  \_sendArgs.SetBuffer(buff, 0, buff.Length); // 실제 있는 Data의 버퍼를 넣어줌.
  }
  \_sendArgs.BufferList = \_pendingList; // 예약된 목록들이 다 들어있다.(BufferList)
  }

  void OnSendCompleted(object sender, SocketAsyncEventArgs args)
  {
  lock(\_lock)
  {
  if(args.BytesTransferred>0 && args.SoccetError == SoccetError.Success)
  {
  try
  {
  // 다음 단계를 보낼 준비
  \_sendArgs.BufferList = null; // \_pendingList를 가지고 있을 필요X
  \_pendingList.Clear();

          // 몇 바이트를 보내는지 출력
          Console.WriteLine($"Transferred bytes: {_sendArgs.BytesTransferred}");

          // 다른 쓰레드가 Enqueue 했던 것을 처리해준다.
          // 내가 보내는 동안에 pending=false; 하기 전에 다른 쓰레드가 예약을 했을 경우 대비
          if(_sendQueue.Count>0)
          {
            RegisterSend();   // 내가 그 쓰레드 것을 처리해준다.
          }
        }
        catch(Exception e)
        {
          // 에러 출력
        }
      }
      else
      {
      // 보내는 것을 실패
      Disconnect();
      }

  }
  }

* 무조건 보내지 않고 상황을 보고 보낼 필요가 있다.
* Receive를 할 때에 비정상적으로 Receive할 경우는 쫓아내야한다.(디도스 공격 방지)
* Send할 때도 한꺼번에 모아서 보내주는 것이 좋다.

---

[Session #4]

- 서버 엔진과 콘텐츠(서버에서 해주는 일)을 분리 작업

* ServerCore는 라이브러리다.(서버 엔진)
* Contents는 Server 프로젝트에서 한다.
* 구현 부분은 숨긴 상태로 사용만 한다.
* 방법: 상속받는 구현 클래스를 만들어 사용한다.(상속을 더 선호(추상화))

ex) 상속 클래스를 만든어 사용하는 방법

- 사용할 Session class를 만든다. 여기선 GameSession을 예로 든다
  class GameSession : Session
  {
  public override void OnConnected(EndPoint endPoint)
  {
  // 연결됨

      // TODO: 구현 처리(여기선 출력)
      byte[] sendBuff = Encoding.UTF8.GetBytes("Welcome to Server");
      Send(sendBuff);
      Thread.Sleep(1000);
      Disconnect();

  }
  public override void OnDisconnected(EndPoint endPoint)
  {
  // Disconnec됨
  }
  public override void OnRecv(ArraySegment<byte> buffer)
  {
  // 받음
  string recvData = Encoding.UTF8.GetString(buffer.Array, buffer.Offset, buffer.Count);
  // TODO : 받고나서 처리
  }
  public override void OnSend(int numOfBytes)
  {
  // 보내는 것을 처리
  }
  }

- Listener Class
  필드 변수 수정
  // Session을 누구로 만들지 정의(무슨 Session인지 모르므로)
  // Action<Socket> \_onAcceptHandler;
  Func<Session> \_sessionFactory;

멤버 함수 수정
void OnAcceptCompleted(Object sender, SocketAsyncEventArgs args)
{
// 성공했으면
if(args.SocketError == SocketError.Success)
{ // GameSession만 만들 수 있는 단점 보완
// 어떤 Session인지 모르므로 root 부모로 받아서 Invoke한다.
Session session = \_sessionFactory.Invoke();
session.Start(args.AcceptSocket); // 외부에서 사용 X

      // 오버라이드된 함수 호출
      session.OnConnected(args.AcceptSocket.RemoteEndPoint);    // 클라이언트가 이 함수를 실행할 때 연결을 끊을 수도 있다.
    }
    else
    { // 성공 못함
      // 에러 이유 출력
    }
    RegisterAccept(args);   // 다음 턴을 위해서

}

- Session 부모 클래스
- 추상 함수를 만든다(자식 클래스가 오버라이드하여 구현할 수 있도록 구현부에 코드를 끼워 넣는다)
- OnCompleted 함수에다 추상 함수 호출

  OnSendCompleted(object sender, SocketAsyncEventArgs args)
  {
  lock(\_lock)
  {
  if(args.BytesTransferred > 0 && args.SocketError == SoccetError.Success)
  {
  try
  {
  // 다음 단계로 보낼 준비
  \_sendArgs.BufferList = null; // \_pendList를 가지고 있을 필요 X
  \_pendingList.Clear(); // bool pending 역할

          // 추상 함수 구현부 호출(실질적인 GameSession이 할 일)
          OnSend(_sendArgs.BytesTransferred);

          if(_sendQueue.Count > 0)
          {
            RegisterSend();   // 내가 그 쓰레드 것을 처리해줌
          }
        }
        catch(Exception e)
        {
          // 에러 이유 출력
        }
      }
      else
      {
        // TODO Disconnect
        Disconnect();
      }

  }
  }

  OnRecvCompleted(object sender, SocketAsyncEventArgs args)
  {
  if(args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
  {
  try
  {
  // TODO(Connect: 데이터 받음)
  OnRecv(new ArraySegment<byte>(args.Buffer, args.Offset, args.BytesTransferred));

        // Receive를 받을 준비를 다시 함
        RegisterRecv(args);
      }
      catch(Exception e)
      {
        // TODO: Disconnect
        Disconnect();
      }

  }
  else
  {
  // TODO: Disconnect
  Disconnect();
  }
  }

public void Disconnect()
{
// Disconnect를 두 번하면 에러나기 때문에 이를 방지
// \_disconnected = 1, original value가 1이면 또 close() 못하도록 함
if(Interlocked.Exchange(ref \_disconnected, 1) == 1)
{
return;
}
OnDisconnected(\_socket.RemoteEndPoint);

// 쫓아냄
\_socket.Shutdown(SocketShutdown.Both); // 예고
\_socket.Close();
}

---

[Connector]

- 공식적인 명칭 X, Listener의 반대 역할(Client에서의)
- Server끼리 통신을 하기 위해서 Connector가 필수적이다.

* Connector 만들기(Client가 Async, non-blocking으로 connect하는 예제)

- 마찬가지로 Async를 위해 Register와 OnCompleted로 구현한다.
  ex)
  public class Connector
  {
  Func<Session> \_sessionFactory;
  public void Connect(IPEndPoint endPoint, Func<Session> sessionFactory)
  {
  // ServerSocket과의 연결
  Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
  \_sessionFactory = sessionFactory; // 어떤 세션과 연결을 할 지 모르므로

      // 이벤트 생성(for Async)
      SocketAsyncEventArgs args = new SocketAsyncEventArgs();
      args.Completed += OnConnectedCompleted;   // 이벤트 등록
      args.RemoteEndPoint = endPoint;
      args.UserToken = socket;      // 넘겨줄 정보를 담는다.(object type이므로 모든 type가능)

      RegisterConnect(args);    // 예약 시작

  }
  }

void RegisterConnect(SocketAsyncEventArgs args)
{
// Socket을 멤버 변수를 두지 않는 이유는 동시다발적으로 socket을 사용하므로
Socket socket = args.UserToken as Socket; // Socket type으로 변환
if(socket == null)
{ // 에러 발생
return;
}
bool pending = socket.ConnectAsync(args);
if(pending == false)
{ // 연결 성공
OnConnectedCompleted(null, args);
}
}

void OnConnectedCompleted(object sender, SocketAsyncEventArgs args)
{ // 성공함 -> 세션 연동
if(args.SocketError == SocketError.Success)
{
// new Session()으로 하는 것이 아닌 콘텐츠 단에서 요구하는 방식대로 만든다.
Session session = \_sessionFactory.Invoke();
session.Start(args.ConnectSocket); // 연결한 소켓
}
else
{
// 에러 이유 출력
}
}

- ServerCore 라이브러리화

* ServerCore는 라이브러리로서 output파일이 exe가 아니라 library파일이다.

1. ServerCore의 .csproj파일 내용에서 기존의 <OutputType>Exe</OutputType>를
   <OutputType>Library</OutputType>으로 수정한다.
2. 실질적으로 실행할 Server Project와 DummyClient에서 사용하기 API를 사용하기 위해서
   각각 .csproj 파일 내용에서
   <ItemGroup>
   <ProjectReference Include="..\ServerCore\ServerCore.csproj"/>
   </ItemGroup>
   을 추가한다.
3. 마지막으로 각각 .cs파일에서 using ServerCore; 를 추가하여 API를 사용한다.
4. 사용할 클래스들을 public으로 해야 사용 가능하다.

---

[TCP vs UDP]

- Packet(패킷)을 주고 받는다.

- TCP vs UDP

* TCP(Transmission Control Protocol)

1. 전화 연결 방식, 안전함
2. 연결형 서비스
3. 전송 순서 보장O
4. 속도 느림(UDP에 비교해서)
5. 신뢰성 높음(분실 일어나면 다시 보냄)
6. 흐름/혼잡 제어

- UDP(User Datagram Protocol): ex) FPS게임

1. 우편 전송 방식, 위험함
2. 비연결형 서비스
3. 전송 순서 보장X
4. 속도 빠름
5. 신뢰성 낮음

---

[RecvBuffer]

- Receive buffer 처리 클래스
  필드 변수: 1)버퍼, 2)읽어 처리한 위치 커서, 3)받아서 쓴 위치 커서

1. buffer
2. read cursor
3. write cursor

멤버 함수:

- [][][][][] : 5 byte

1. void Clean(): Read, Write Cursor를 버퍼가 빈 상태로 초기화한다. ex) [rw][][][][]: 버퍼가 빈 상태
   1.1) 남은 데이터가 없으면 커서 위치만 리셋(r, w cursor가 겹친 상태) ex) [][][][rw][] -> [rw][][][][]
   1.2) 남은 데이터가 있으면 시작위치로 복사 ex) [][r][][w][] -> [r][][w][][]

2. bool OnRead(int numOfBytes): Read를 했을 때를 처리(에러 or 성공: Read Cursor를 옮김)
   ex) 2 byte씩 읽어서 처리 할 때, [r][][][][w] -> [][][r][][w]

3. bool onWrite(int numOfBytes): Write를 했을 때를 처리(에러 or 성공: Write Cursor를 옮김)
   ex) 3byte를 받아서 써야 할 때, [r][w][][][] -> [r][][][][w]

ex) Receive buffer class
public class RecvBuffer // 라이브러리로 사용할 수 있도록 public
{
ArraySegment<byte> \_buffer; // C++에서의 pointer로 하는 것을 C#에서는 Segment로 처리하기 위한 변수 선언
int \_readPos; // Read Cursor
int \_writePos; // Write Cursor

public RecvBuffer(int bufferSize) // 생성자
{
\_buffer = new ArraySegment<byte>(new byte[bufferSize], 0, bufferSize);
}

public int DataSize // 쌓여있는(처리해야 할) Data 크기
{
get{ return (\_writePos - \_readPos); }
}
public int FreeSize // 버퍼의 남은 공간
{
get{ return (\_buffer.Count - \_writePos); }
}

public ArraySegment<byte> ReadSegment // 처리할 세그먼트 리턴
{
get{ return new ArraySegment<byte>(\_buffer.Array, \_buffer.Offset+\_readPos, DataSize); }
}
public ArraySegment<byte> WriteSegment // 남은 공간 세그먼트 리턴
{
get{ return new ArraySegment<byte>(\_buffer.Array, \_buffer.Offset+\_writePos, FreeSize); }
}

public void Clean()
{
int dataSize = DataSize;
if(dataSize == 0) // r, w cursor가 겹쳤는가?
{ // 남은 데이터가 없으면 복사하지 않고 커서 위치만 리셋
\_readPos = \_writePos = 0;
}
else
{ // 남은 데이터가 있으면 시작 위치로 복사한다.
// [][][r][][w]
// Copy(SourceArray, Source위치, DestinationArray, Destination위치, 복사할 크기)
Array.Copy(\_buffer.Array, \_buffer.Offset+\_readPos, \_buffer.Array, \_buffer.Offset, dataSize);

        // [r][][w][][]
        _readPos = 0;
        _writePos = dataSize;
    }

}

public bool OnRead(int numOfBytes) // Read를 했을 때
{
if(numOfBytes > DataSize)
{ // 비정상
return false;
}
// Read Cursor를 옮김
\_readPos += numOfBytes;
return true;
}
public bool OnWrite(int numOfBytes) // Recv를 했을 때
{
if(numOfBytes > FreeSize)
{ // 비정상
return false;
}
// Write Cursor를 옮김
\_writePos += numOfBytes;
return true;
}
}

- in Session 클래스

* 버퍼 선언
  RecvBuffer \_recvBuffer = new RecvBuffer(1024);

* Recv 예약함수 안에서는 처음 부분에,
  \_recvBuffer.Clean(); // 커서가 너무 뒤로 이동하는 것을 방지, 버퍼 정리

* Recv완료 함수에서는 데이터를 받았을 때,

1. Write 커서 이동
   if(\_recvBuffer.OnWrite(args.BytesTransferred) == false)
   { // 버그 발생
   Disconnect();
   return;
   }

2. 컨텐츠 단으로 데이터 범위 만큼만 넘기고 얼마나 처리 했는지 받는다.
   int processLength = OnRecv(\_recvBuffer.ReadSegment);
   if(processLength < 0 || \_recvBuffer.DataSize < processLength)
   { // 콘텐츠 단에서 이상하게 넣었을 경우
   Disconnect();
   return ;
   }

3. Read 커서 이동
   if(\_recvBuffer.OnRead(processLength) == false)
   { // 버그 발생
   Disconnect();
   return;
   }

---

[SendBuffer]

- Send buffer

* Send는 Recv와는 다르게 모든 사용자에게 동일한 상황을 주기 위해 많이 실행된다.
  따라서, Send buffer를 Copy해서 사용하게 되면 Copy의 비용이 너무 들기 때문에 최적화가 필요하다.
* 아주 큰 사이즈로(Chunk size)로 덩어리처럼 만들고 쪼개서 사용하는 식으로 만든다.

ex) SendBuffer 클래스와 SendBufferHelper 클래스
public class SendBuffer // 실질적으로 Helper class를 사용(일회용)
{
byte[] buffer;
int \_usedSize = 0; // 일종의 사용한 만큼을 나타내는 커서가

    public SendBuffer(int chunkSize)
    {
      _buffer = new byte[chunkSize];
    }

    public int FreeSize{ get{ return _buffer.Length - _usedSize; } }  // 남은 공간

    public ArraySegment<byte> Open(int reserveSize)   // 매개 변수로 예약 공간을 받아 처리한다.(일종의 예약)
    {
      if(reserveSize > FreeSize)
      {
        return null;
      }
      return new ArraySegment<byte>(_buffer, _usedSize, reserveSize);
    }

    public ArraySegment<byte> Close(int usedSize)   // 매개 변수로 사용한 사이즈를 처리한다.(일종의 완료)
    {
      ArraySegment<byte> segment = new ArraySegment<byte>(_buffer, _usedSize, usedSize);
      _usedSize += usedSize;    // 커서 옮김
      return segment;
    }

}

public class SendBufferHelper
{ // 쓰레드끼리의 경합을 없앤다.
public static ThreadLocal<SendBuffer> CurrentBuffer = new ThreadLocal<SendBuffer>(() => { return null; } );
public static int ChunkSize{ get; set; } = 4096 \* 100; // 크게 잡고 분할해서 쓴다.

    public static ArraySegment<byte> Open(int reserveSize)
    {
      if(CurrentBuffer.Value == null)   // 한 번도 사용 안했을 때
      {
        CurrentBuffer.Value = new SendBuffer(ChunkSize);
      }
      if(CurrentBuffer.Value.FreeSize < reserveSize)  // 남은 공간보다 예약할 공간이 더 많을 때
      {
        CurrentBuffer.Value = new SendBuffer(ChunkSize);
      }
      return CurrentBuffer.Value.Open(reserveSize);   // 남은 공간이 있어서 덩어리 쪼개서 사용
    }

    public static ArraySegment<byte> Close(int usedSize)
    {
      return CurrentBuffer.Value.Close(usedSize);
    }

}

- Server Class에서의 사용
  ex) 연결되고 후처리로
  ArraySegment<byte> openSegment = SendBufferHelper.Open(4096);
  byte[] buffer1 = BitConverter.GetBytes(int ...); // int type을 byte 배열로 만든다.
  byte[] buffer2 = BitConverter.GetBytes(int ...);

  // buffer1 + buffer2를 openSegment에 복사한다.
  Array.Copy(buffer1, 0, openSegment.Array, openSegment.Offset, buffer1.Length);
  Array.Copy(buffer2, 0, openSegment.Array, openSegment.Offset + buffer1.Length, buffer2.Length);

  // 보낼 버퍼 완료
  ArraySegment<byte> sendBuff = SendBufferHelper.Close(buffer1.Length + buffer2.Length);

  // 전송
  Send(sendBuff);

---

[PacketSession]

- Packet

* 실질적으로는 Data를 Packet을 주고 받는다
* 패킷은 최대한 압축해서 보내야된다.
* Pack의 기본적인 헤더는 다음과 같다.
  class Packet // 공통적인 것을 멤버 변수로 하고 이 클래스를 상속 받는다.
  {
  public ushort size; // ushort(2byte) vs uint(4byte)
  public ushort packetId; // 무슨 패킷인지 구별
  }
  이다.
* 이 클래스를 상속받아서 사용한다.
* 자식 클래스는 size를 모를 수 있으므로 size가 멤버 변수로 선언된다.

- PacketSession

* Packet Session을 만들어 패킷을 자동으로 유효 범위로만 파싱될 수 있도록 한다.

ex)
public abstract class PacketSession : Session
{
// [size(2)][packetid(2)][option], [size(2)][packetid(2)][option], ...
public static readonly int HeaderSize = 2; // HeaderSize를 읽기 전용으로 2 byte로 칭한다.

// sealed 키워드: 이 클래스의 자식 클래스가 오버라이드 못한다(봉인)
public sealed override int OnRecv(ArraySegment<byte> buffer)
{
int processLength = 0;

    while(ture)
    {
      if(buffer.Count < HeaderSize)   // 최소한 헤더는 파싱할 수 있는지 확인
      {
        break;
      }

      // 패킷이 완전체로 도착했는지 확인
      ushort dataSize = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
      if(buffer.Count < dataSize)
      {
        break;
      }

      // 여기부터는 패킷 조합 가능
      // 패킷의 유효 범위만을 넘겨준다.(사이즈 전체 or 패킷 내용만(헤더 빼고))
      // 넘겨줄 때 방식: 1) buffer.Slice() or 2) new ArraySegment<byte>: 힙 영역이 아니므로 같은 결과
      OnRecvPacket(new ArraySegment<byte>(buffer.Array, buffer.Offset, dataSize));

      processLength += dataSize;
      buffer = new ArraySegment<byte>(buffer.Array, buffer.offset + dataSize, buffer.Count - dataSize);
    }
    return processLength;

}
// 처리할 바이트 수(콘텐츠 단으로 유효한 패킷만 넘김)
// PacketSession을 상속 받는 클래스는 이 인터페이스를 오버라이드한다.
public abstract void OnRecvPacket(ArraySegment<byte> buffer);
}

- Server에서는 Packet packet = new Packet() { size = N, packetId = N }; 으로 선언하여 사용한다.

---

[Serialization #1]

- 직렬화 vs 역직렬화

* Serialization(직렬화) : 객체 형태의 패킷을 버퍼 안에 밀어넣는 작업
* Deserialization(역직렬화) : 버퍼 안에 패킷을 객체 형태로 만드는 작업

- Packet 정의

* 서버, 클라이언트는 동시에 패킷 정보를 알고 있어야된다.
* 패킷은 .json, .xml 파일을 만들어서 자동화하여 정의한다.

* 서버끼리 연결되는 분산 서버의 경우는 Session을 나눠서 코드 작성한다.
  ex) ClientSession : in Server
  class ClientSession : PacketSession
  {
  public override void OnConnected(EndPoint endPoint)
  {
  // 연결됨
  // 연결되고 후 처리
  // 연결 끊음
  }

      public override void OnRecvPacket(ArraySegment<byte> buffer)
      {
        // Deserialization
        ushort count = 0;   // 자동화하기 편리하도록 변수 선언
        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += 2;     // 하드 코딩(나중에 변경) -> sizeof(ushort);
        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += 2;

        // Parsing
        switch((PacketID)id)
        {
          case PacketID.PlayerInfoReq:
            {
              long playerId = BitConverter.ToInt64(buffer.Array, buffer.Offset+count);
              count+=8;
            }
            break;
        }
      }

      public override void OnDisconnected(EndPoint endPoint)
      {
        // 연결 종료 후 처리
      }

      public override void OnSend(int numOfBytes)
      {
        // Send() 후 처리
      }

  }

ex) ServerSession : in Client
class ServerSession : Session
{
public override OnConnected(EndPoint endPoint)
{
// 연결됨
// 연결되고 후 처리
ArraySegment<byte> seg = SendBufferHelper.Open(4096); // 4096 byte 공간 예약

      ushort count = 0;   // 자동화하기 편리하도록 변수 선언
      bool success = true;

      // span<byte> : 범위를 지정할 수 있다.
      // 실패(공간 모자름) vs 성공(거의 다) --- Serialization(version 1)
      count+=2;
      success &= BitConverter.TryWriteBytes(new Span<byte>(seg.Array, seg.Offset+count, seg.Count-count), packet.packetId);
      count+=2;
      success &= BitConverter.TryWriteBytes(new Span<byte>(seg.Array, seg.Offset+count, seg.Count-count), packet.packetId);
      count+=8;
      success &= BitConverter.TryWriteBytes(new Span<byte>(seg.Array, seg.Offset+count, seg.Count), count;    // 패킷 헤더

      // Size는 마지막에 알 수 있다.
      ArraySegment<byte> sendBuff = SendBufferHelper.Close(count);
      if(success){ send(sendBuff); }
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
      // 연결 끊음 후 처리
    }

    public override int OnRecv(ArraySegment<byte> buffer)
    {
      // (버퍼, offset(시작 인덱스), 받은 바이트 수)
      string recvData = Encoding.UTF8.GetString(buffer.Array, buffer.Offset, buffer.Count);
      return buffer.Count;    // 받은 바이트 수 리턴
    }

    public override void OnSend(int numOfBytes)
    {
      // 보내고 후 처리
      // 보낸 바이트수 출력 등
    }

}

- Serialization(version 2) : unsafe 코드 이용(C++처럼 포인터 접근 가능하다)
  static unsafe void ToBytes(byte[] array, int offset, ulong value)
  {
  fixed(byte* ptr = &array[offset])
  *(ulong\*)ptr = value;
  }

- Serialization(version 3) : 비트 연산 노가다

---

[Serialization #2]

- 직렬화를 따로 작성하여 재사용성을 높인다.

ex) Packet 추상 클래스
public abstract class Packet
{
public ushort size; // 경우에 따라서 제거, ushort(2) vs uint(4)
public ushort packetId; // 경우에 따라서 제거, ushort(2) vs uint(4)

    public abstract ArraySegment<byte> Write();
    public abstract void Read(ArraySegment<byte> buffer);

}

ex) PlayerInfoReq 패킷 예시
class PlayerInfoReq : Packet // Client -> Server
{
public long playerId;

    public PlayerInfoReq()      // 생성자
    {
      this.packetId = (ushort)PacketID.PlayerInfoReq;
    }

    public override void Read(ArraySegment<byte> buffer)
    {
      // Deserialization
      ushort count = 0;
      count += 2;
      count += 2;

      // this.playerId = BitConverter.ToInt64(buffer.Array, buffer.Offset + count);    // 필드 변수 직접 할당
      // 범위를 초과하는 값을 Parsing하는 것을 Exception을 발생시켜 Option을 부과할 수 있다.
      this.playerId = BitConverter.ToInt64(new ReadOnlySpan<byte>(buffer.Array, buffer.Offset+count, buffer.Count - count));
      count+=8;
    }

}

- Server가 Client의 헤더 신뢰성

* 패킷 헤더에 있는 정보는 참고만 해야된다.
* 유효 범위를 지정했을 뿐, 메모리에 데이터는 있다.
* BitConverter로 강제로 추출하면 추출은 가능하다.
* 충분한 공간이 있는지 계속 검사(BitConverter의 new ReadOnlySpan으로 검사)
* 패킷 헤더가 변조되어 검사에서 초과 Exception이 발생하면 Disconnect()를 처리한다.

---

[UTF-8 vs UTF-16]

- ENCODING

* Encoding: 문자를 컴퓨터가 이해시키기 위해 만들었다.

- ASCII Code vs UNICODE

* ASCII Code(1Byte): 0~127, 점점 부족함, ex) 'A' = 0x41(65), '!' = 0x21(33)
* UNICODE(2Byte -> 3Byte): 0~65535, 점점 더 부족하여 3Byte로 늘렸다.

- UTF-8 vs UTF-16

* UTF-8

1. 가변적임
2. 영문은 1Byte, 한글은 3Byte로 표현된다.

- UTF-16

1. 가변적임
2. BMP까지는 2Byte, 그 다음은 4Byte
3. C#은 기본적으로 UTF-16이다.
4. 영문 2Byte, 한글 2Byte로 표현된다.

- Client와 Server는 UTF-8 or UTF-16을 맞춰야 통신이 가능하다.

---

[Serialization #3]

- 고정적인 데이터(자료형)가 아닌 가변적인 데이터 처리

* string 처리: C++은 \0로 판별, UTF-16 기준에서 length[2byte]와 byte[string]을 보낸다.
  ex) Read와 Write (string)
  class PlayerInfoReq : Packet
  {
  public string name;

      public override void Read(ArraySegment<byte> segment)
      {
        ushort count = 0;

        // Slice 위해 필요
        ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(segment.Array, segment.Offset, segment.Count);

        // string parsing
        // length[2byte]
        ushort nameLen = BitConverter.ToUInt16(Span.Slice(count, span.Length - count));
        count += sizeof(ushort);

        // Decoding
        // Unicode(UTF-16)
        this.name = Encoding.Unicode.GetString(span.Slice(count, nameLen));
      }

      public override ArraySegment<byte> Write()
      {
        ArraySegment<byte> segment = SendBufferHelper.Open(4096);

        ushort count = 0;
        bool success = true;

        Span<byte> span = new Span<byte>(segment.Array, segment.Offset, segment.Count);

        // 첫 번째 방법
        // length[2byte]
        ushort nameLen = (ushort)Encoding.Unicode.GetByteCount(this.name);
        success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), nameLen);
        count += sizeof(ushort);

        // byte[string]
        Array.Copy(Encoding.Unicode.GetBytes(this.name), 0, segment.Array, count, nameLen);
        count += nameLen;

        // 두 번째 방법 : segment.Offset + count + sizeof(ushort)은 미리 시작 위치에서 count + length길이
        ushort nameLen = (ushort)Encoding.Unicode.GetBytes(this.name, 0, this.name.Length, segment.Array, segment.Offset + count + sizeof(ushort));
        success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), nameLen);
        count += sizeof(ushort);
        count += nameLen;

        success &= BitConverter.TryWriteBytes(span, count);   // 마지막 최종 카운트

        if(success == false) { return null; }

        // 사이즈를 마지막에 알 수 있다.
        return SendBufferHelper.Close(count);
      }

  }

* 항상 Packet 조작 위험을 조심해야한다.

---

[Serialization #4]

- List<T> 직렬화: string과 다르게 Type도 가변적이며, 길이도 가변적이다.

* string과 마찬가지로 개수와 같이 보내준다.
* 구조체가 있다면 함수를 안에 만들어 자동화시킨다.

ex) 구조체 SkillInfo를 갖는 List<SkillInfo>
public class PlayerInfoReq : Packet
{
public struct SkillInfo
{
public int id;
public short level;
public float duration;

    public bool Write(Span<byte> span, ref ushort count)    // count를 참조자로 받아서 값을 넣는다.
    {
      bool success = true;

      success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), id);
      count += sizeof(int);

      success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), level);
      count += sizeof(short);

      success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), duration);
      count += sizeof(float);

      return success;
    }

    public void Read(ReadOnlySpan<byte> span, ref ushort count)
    {
      id = BitConverter.ToInt32(span.Slice(count, span.Length - count));
      count += sizeof(int);

      level = BitConverter.ToInt16(span.Slice(count, span.Length - count));
      count += sizeof(short);

      // Single: float, Double: double
      duration = BitConverter.ToSingle(span.Slice(count, span.Length - count));
      count += sizeof(float);
    }

}

// 구조체 정의가 끝난 후 선언
public List<SkillInfo> skills = new List<SkillInfo>();

public override ArraySegment<byte> Write()
{
ArraySegment<byte> segment = SendBufferHelper.Open(4096); // 공간 예약

    ushort count;     // for 자동화
    bool success = true;    // for 범위 초과 예외처리

    Span<byte> span = new Span<byte>(segment.Array, segment.Offset, segment.Count);

    // skill list
    success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), (ushort)skills.count);
    count += sizeof(ushort);

    // 스킬 모두 Serialize
    foreach(SkillInfo skill in skills)
    {
      success &= skill.Write(span, ref count);
    }

    success &= BitConverter.TryWriteBytes(span, count);   // 최종 카운트

    if(success == false) { return null; }

    return SendBufferHelper.Close(count);

}

public override void Read(ArraySegment<byte> segment)
{
ushort count = 0;

    ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(segment.Array, segment.Offset, segment.Count);

    // Parsing skill list
    ushort skillLen = BitConverter.ToUInt16(span.Slice(count, span.Lengt - count));
    count += sizeof(ushort);

    skills.Clear();   // 혹시나 다른 정보가 있을까봐
    for(int i=0; i<skillLen; i++)
    {
      SKillInfo skill = new SkillInfo();
      skill.Read(span, ref count);

      // 추가
      skills.Add(skill);
    }

}
}

- Protocol buffers vs Flat buffers

* Protocol buffers: 중간에 인스턴스를 만들어 채우고 변환하는 작업(직관적임), 구조화된 데이터를 직렬화
* Flat buffers: 데이터를 배열에다가 바로 집어넣는 작업, 플랫폼에 상관없다.(게임에 많이 쓴다)

---

[Packet Generator #1]

- 패킷 생성 자동화

* 패킷 정의

1. .json
2. .xml
3. IDL: 인터페이스 정의 언어

ex 1)
PDL.xml 생성(Packet Definition List)

<?xml version="1.0" encoding "utf-8" ?>
<PDL>
    <packet name="PlayerInfoReq">
      <long name="playerId"/>
      <string name="name"/>
      <list name="skill">
        <int name="id"/>
        <short name="level"/>
        <float name="duration"/>
      </list>
    </packet>
  </PDL>
- <packet> ~ </packet> : 안에 내용이 많으면
- < ~ /> : 한 줄짜리 내용이면
- long, string, float 등은 나중에 parsing을 위해서 정해주는 것이다.
- xml파일은 default로 읽을 때, 실행 파일 위치에서 읽는다.

ex 2) Packet Generator 프로그램 작성
class Program
{
static void Main(string[] args)
{
XmlReaderSettings settings = new XmlReaderSettings() // 환경 설정
{
IgnoreComments = true, // 주석 무시
IgnoreWhiteSpace = true // 스페이스 바 무시
}; // ';' 필수

      using(XmlReader reader = XmlReader.Create("PDL.xml", settings))   // using으로 하면 나중에 reader.Dispose(); 를 자동으로 해준다.
      {
        reader.MoveToContent();     // 헤더를 건너뛰고 핵심 내용으로 바로 들어간다.

        while(reader.Read())        // string 방식으로 읽어드린다.
        {
          if(reader.Depth==1 && reader.MoveType == XmlNodeType.Element)   // Element: 시작, EndElement: 끝
            ParsePacket();    // 패킷 파싱
        }
      }
    }

    public static void ParsePacket(XmlReader reader)
    {
      if(reader.NodeType == XmlNodeType.EndElement) return;   // 끝일 경우 들어옴
      if(reader.Name.ToLower() != "packet") return;   // 소문자로 변환 후 packet이 아니면 return

      string packetName = reader["name"];
      if(string.IsNullOrEmpty(PacketName)) return;    // 비어있을 경우

      ParseMembers(reader);   // (세부적인)멤버 파싱
    }

    public static void ParseMembers(XmlReader reader)
    {
      string packetName = reader["name"];

      int depth = reader.Depth + 1;   // packet의 depth + 1
      while(reader.Read())
      {
        if(reader.Depth != depth) break;      // packet 안의 내용이 아닐 경우

        string memberName = reader["name"];
        if(string.IsNullOrEmpty(memberName)) return;

        string memberType = reader.Name.ToLower();    // 소문자 변환
        switch(memberType)    // 타입 파싱
        {
          case "bool":
          case "byte":
          case "short":
          case "ushort":
          case "int":
          case "long":
          case "float":
          case "double":
          case "string":
          case "list":
            break;
          default:
            break;
        }
      }
    }

}

- Contents: XmlReader reader의 ["..."]

ex 3) PacketFormat 바꿔치기 클래스 작성
class PacketFormat
{
// {0} 패킷 이름
// {1} 멤버 변수들
// {2} 멤버 변수 Read
// {3} 멤버 변수 Write
public static string packetFormat =
@"
class {0}
{{
{1}

    public void Read(ArraySegment<byte> segment)
    {{
      // Deserialization
      ushort count = 0;
      ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(segment.Array, segment.Offset, segment.Count);
      // ushort size = BitConverter.ToUInt16(seg.Array, seg.Offset);
      count += sizeof(ushort);
      // ushort id = BitConverter.ToUInt16(seg.Array, seg.Offset + count);
      count += sizeof(ushort);
      {2}
    }}

    public ArraySegment<byte> Write()
    {{
      // 연결되고 후처리
      ArraySegment<byte> segment = SendBufferHelper.Open(4096);       // 공간 예약
      ushort count = 0;       // 자동화하기 편리하게
      bool success = true;
      Span<byte> span = new Span<byte>(segment.Array, segment.Offset, segment.Count);     // for Slice
      // 실패, 성공 여부가 갈림---version 1, 공간이 모자르면 실패
      count += sizeof(ushort);        // 처음 패킷Id
      success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), (ushort)PacketID.{0});
      count += sizeof(ushort);     // 나중에 자동화
      {3}
      success &= BitConverter.TryWriteBytes(span, count);         // 마지막 최종 카운트
      if (success == false)
        return null;

      // 사이즈는 마지막에 알 수 있다.
      return SendBufferHelper.Close(count);
    }}

}}
";

      // {0} 변수 형식
      // {1} 변수 이름
      public static string memberFormat =

@"
public {0} {1}
";

      // {0} 변수 이름
      // {1} To~ 변수 형식
      // {2} 변수 형식
      public static string readFormat =

@"
this.{0} = BitConverter.{1}(span.Slice(count, span.Length - count));
count += sizeof({2});
";

      // {0} 변수 이름
      public static string readStringFormat =

@"
ushort {0}len = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
count += sizeof(ushort);
this.{0} = Encoding.Unicode.GetString(span.Slice(count, {0}Len));
count += {0}Len;
";
// {0} 변수 이름
// {1} 변수 형식
public static string writeFormat =
@"
success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), this.{0});
count += sizeof({1});
";

      // {0} 변수 이름
      public static string writeStringFormat =

@"
ushort {0}Len = (ushort)Encoding.Unicode.GetBytes(this.{0}, 0, this.{0}.Length, segment.Array, segment.Offset + count + sizeof(ushort))
success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), {0}Len);
count += sizeof(ushort);
count += {0}Len;
";
}

- 패킷의 포맷을 정의해주고 안에 있는 내용을 바꿔 치기한다.
- @""를 통해서 길게 string을 지정한다.
- {}는 한 번씩 추가해서 넣어준다. ex) {{ ... }}

---

[Packet Generator #2]

- 패킷 코드 생성(코드를 생성한다)

* 패킷 포맷 클래스
  {
  // 기본적인 패킷 포맷
  // 패킷의 기본적인 멤버 포맷
  // 패킷의 리스트 멤버 포맷
  // 패킷 기본적인 읽기
  // 패킷 기본적인 쓰기
  // 패킷 스트링 읽기
  // 패킷 스트링 쓰기
  // 패킷 리스트 읽기
  // 패킷 리스트 쓰기
  }

* 실행 프로그램 클래스
  {
  // 실시간으로 만들어지는 패킷 코드 변수
  // 메인 함수
  // Xml 환경설정 변수
  // XmlReader using 사용해서 read
  // 포맷 파싱
  // (세부적인)멤버 파싱: 포맷 파싱에서 호출
  }

ex 1) PacketFormat 클래스
class PacketFormat
{
// {0} 패킷 이름
// {1} 멤버 변수들
// {2} 멤버 변수 Read
// {3} 멤버 변수 Write
public static string packetFormat =
@"
class {0}
{{
{1}

    public void Read(ArraySegment<byte> segment)
    {{
      ushort count = 0;

      ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(segment.Array, segment.Offset, segment.Count);
      count += sizeof(ushort);
      count += sizeof(ushort);
      {2}
    }}

    public ArraySegment<byte> Write()
    {{
      ArraySegment<byte> segment = SendBufferHelper.Open(4096);       // 공간 예약

      ushort count = 0;       // 자동화하기 편리하게
      bool success = true;

      Span<byte> span = new Span<byte>(segment.Array, segment.Offset, segment.Count);     // for Slice

      count += sizeof(ushort);        // 처음 패킷Id
      success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), (ushort)PacketID.{0});
      count += sizeof(ushort);     // 나중에 자동화
      {3}
      success &= BitConverter.TryWriteBytes(span, count);         // 마지막 최종 카운트
      if (success == false)
        return null;

      return SendBufferHelper.Close(count);
    }}

}}
";

    // {0} 변수 형식
    // {1} 변수 이름
    public static string memberFormat =

@"
public {0} {1};
";

    // {0} 리스트 이름 [대문자]
    // {1} 리스트 이름 [소문자]
    // {2} 멤버 변수들
    // {3} 멤버 변수 Read
    // {4} 멤버 변수 Write
    public static string memberListFormat =

@"
public struct {0}
{{
{2}

    public void Read(ReadOnlySpan<byte> span, ref ushort count)
    {{
        {3}
    }}

    public bool Write(Span<byte> span, ref ushort count)
    {{
        bool success = true;
        {4}
        return success;
    }}

}}
public List<{0}> {1}s = new List<{0}>();
";

    // {0} 변수 이름
    // {1} To~ 변수 형식
    // {2} 변수 형식
    public static string readFormat =

@"
this.{0} = BitConverter.{1}(span.Slice(count, span.Length - count));
count += sizeof({2});
";

    // {0} 변수 이름
    public static string readStringFormat =

@"
ushort {0}len = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
count += sizeof(ushort);
this.{0} = Encoding.Unicode.GetString(span.Slice(count, {0}Len));
count += {0}Len;
";
// {0} 리스트 이름 [대문자]
// {1} 리스트 이름 [소문자]
public static string readListFormat =
@"
{1}s.Clear();
ushort {1}Len = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
count += sizeof(ushort);
for (int i = 0; i < {1}Len; i++)
{{
    {0} {1} = new {0}();
    {1}.Read(span, ref count);
    {1}.Add({1});
  }}
";
// {0} 변수 이름
// {1} 변수 형식
public static string writeFormat =
@"
success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), this.{0});
count += sizeof({1});
";

    // {0} 변수 이름
    public static string writeStringFormat =

@"
ushort {0}Len = (ushort)Encoding.Unicode.GetBytes(this.{0}, 0, this.{0}.Length, segment.Array, segment.Offset + count + sizeof(ushort));
success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), {0}Len);
count += sizeof(ushort);
count += {0}Len;
";

    // {0} 리스트 이름 [대문자]
    // {1} 리스트 이름 [소문자]
    public static string writeListFormat =

@"
success &= BitConverter.TryWriteBytes(segment.Slice(count, span.Length - count), (ushort){1}.Count);
count += sizeof(ushort);
foreach ({0} {1} in this.{1}s)
success &= {1}.Write(span, ref count);
";
}

ex 2) 실행 프로그램 코드
class Program
{
static string genPackets; // 실시간으로 만들어지는 패킷 코드
static void Main(string[] args)
{
XmlReaderSettings settings = new XmlReaderSettings() // 환경 설정
{
IgnoreComments = true, // 주석 무시
IgnoreWhitespace = true // 스페이스 바 무시
};

    using (XmlReader reader = XmlReader.Create("PDL.xml", settings))
    {
      reader.MoveToContent();      // 헤더를 건너뛰고 핵심 내용으로 바로 들어감

      while (reader.Read())        // string 방식으로 읽어드림
      {
        if (reader.Depth == 1 && reader.NodeType == XmlNodeType.Element)    // Element 시작, EndElement 끝
        {
            ParsePacket(reader);
        }
        // System.Console.WriteLine(reader.Name + " " + reader["name"]);     // ["name"] : name이라는 콘텐츠만 읽는다.
      }

      File.WriteAllText("GenPacket.cs", genPackets);    // 조립
    }

      // reader.Dispose();        // 사용을 닫아줌 or using 사용(알아서 범위 내에서 Dispose 호출)
    }

    public static void ParsePacket(XmlReader reader)
    {
      if (reader.NodeType == XmlNodeType.EndElement) return; // 잘못 들어옴
      if (reader.Name.ToLower() != "packet")          // 소문자로 변환 후 packet이 아니면 return
      {
        System.Console.WriteLine("Invalid packet node");
        return;
      }

      string packetName = reader["name"];
      if (string.IsNullOrEmpty(packetName))       // 비어있는지
      {
        System.Console.WriteLine("Packet without name");
        return;
      }

      Tuple<string, string, string> tuple = ParseMembers(reader);
      genPackets += string.Format(PacketFormat.packetFormat, packetName, tuple.Item1, tuple.Item2, tuple.Item3);
    }

    // {1} 멤버 변수들
    // {2} 멤버 변수 Read
    // {3} 멤버 변수 Write
    public static Tuple<string, string, string> ParseMembers(XmlReader reader)
    {
      string packetName = reader["name"];

      string memberCode = "";     // For Tuple
      string readCode = "";       // For Tuple
      string writeCode = "";      // For Tuple

      int depth = reader.Depth + 1;       // packet의 depth+1
      while (reader.Read())
      {
        if (reader.Depth != depth) break;
        string memberName = reader["name"];
        if (string.IsNullOrEmpty(memberName))
        {
          System.Console.WriteLine("Member without name");
          return null;
        }
        if (string.IsNullOrEmpty(memberCode) == false)
        {
          memberCode += Environment.NewLine;      // 내용이 있으면 개행
        }

        string memberType = reader.name.ToLower();
        switch (memberType)
        {
          case "bool":
          case "byte":
          case "short":
          case "ushort":
          case "int":
          case "long":
          case "float":
          case "double":
            memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
            readCode += string.Format(PacketFormat.readFormat, memberName, ToMemberType(memberType), memberType);
            writeCode += string.Format(PacketFormat.writeFormat, memberName, memberType);
            break;
          case "string":
            memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
            readCode += string.Format(PacketFormat.readStringFormat, memberName);
            writeCode += string.Format(PacketFormat.writeStringFormat, memberName);
            break;
          case "list":
            Tuple<string, string, string> tuple = ParseList(reader);
            memberCode += tuple.Item1;
            readCode += tuple.Item2;
            writeCode += tuple.Item3;
            break;
          default:
            break;
        }
      }
      // 정렬을 위해서
      memberCode = memberCode.Replace("\n", "\n\t");      // ("\n"을 \"n\t"으로 바꿈)
      readCode = readCode.Replace("\n", "\n\t\t");
      writeCode = writeCode.Replace("\n", "\n\t\t");

      // 한 번에 세 개를 반환
      return new Tuple<string, string, string>(memberCode, readCode, writeCode);
    }

      public static Tuple<string, string, string> ParseList(XmlReader reader)
      {
        string listName = reader["name"];
        if (string.IsNullOrEmpty(listName))
        {
          System.Console.WriteLine("List without name");
          return null;
        }

        Tuple<string, string, string> tuple = ParseMembers(reader);

        string memberCode = string.Format(PacketFormat.memberListFormat,
          FirstCharToUpper(listName),
          FirstCharToLower(listName),
          tuple.Item1,
          tuple.Item2,
          tuple.Item3
        );

        string readCode = string.Format(PacketFormat.readListFormat,
          FirstCharToUpper(listName),
          FirstCharToLower(listName)
        );

        string writeCode = string.Format(PacketFormat.writeListFormat,
          FirstCharToUpper(listName),
          FirstCharToLower(listName)
        );

        return new Tuple<string, string, string>(memberCode, readCode, writeCode);
      }

      public static string ToMemberType(string memberType)    // 멤버 타입 파싱
      {
        switch (memberType)
        {
          case "bool":
            return "ToBoolean";
          case "short":
            return "ToInt16";
          case "ushort":
            return "ToUINT16";
          case "int":
            return "ToInt32";
          case "long":
            return "ToInt64";
          case "float":
            return "ToSingle";
          case "double":
            return "ToDouble";
          default:
            return "";
        }
      }

      public static string FirstCharToUpper(string input)   // 첫 글자만 Upper로
      {
        if (string.IsNullOrEmpty(input)) return "";
        return input[0].ToString().ToUpper() + input.Substring(1);
      }

      public static string FirstCharToLower(string input)   // 첫 글자만 Lower로
      {
        if (string.IsNullOrEmpty(input)) return "";
        return input[0].ToString().ToLower() + input.Substring(1);
      }

}

- 패킷 인터페이스 코드를 생성한다.

---

[Packet Generator #3]

- packet ID를 나타내는 enum 값, using 값을 자동화시킨다.

* byte, sbyte 지원
  ex 1) PacketFormat class
  class PacketFormat
  {
  // {0} 패킷 이름/번호 목록
  // {1} 패킷 목록
  public static string fileFormat = // using과 enum값
  @"using System;
  using System.Collections.Generic;
  using System.Text;
  using System.Net;
  using ServerCore;

public enum PacketID
{{
    {0}
}}

{1}
";
// {0} 패킷 이름
// {1} 패킷 번호
public static string packetEnumFormat =
@"{0} = {1},";

        // {0} 패킷 이름
        // {1} 멤버 변수들
        // {2} 멤버 변수 Read
        // {3} 멤버 변수 Write
        public static string packetFormat =

@"class {0}
{{
{1}

    public void Read(ArraySegment<byte> segment)
    {{
        ushort count = 0;

        ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(segment.Array, segment.Offset, segment.Count);
        count += sizeof(ushort);
        count += sizeof(ushort);
        {2}
    }}

    public ArraySegment<byte> Write()
    {{
        ArraySegment<byte> segment = SendBufferHelper.Open(4096);       // 공간 예약

        ushort count = 0;       // 자동화하기 편리하게
        bool success = true;

        Span<byte> span = new Span<byte>(segment.Array, segment.Offset, segment.Count);     // for Slice

        count += sizeof(ushort);        // 처음 패킷Id
        success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort)PacketID.{0});
        count += sizeof(ushort);     // 나중에 자동화
        {3}
        success &= BitConverter.TryWriteBytes(span, count);         // 마지막 최종 카운트
        if (success == false)
            return null;

        return SendBufferHelper.Close(count);
    }}

}}
";

        // {0} 변수 형식
        // {1} 변수 이름
        public static string memberFormat =

@"public {0} {1};";

        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        // {2} 멤버 변수들
        // {3} 멤버 변수 Read
        // {4} 멤버 변수 Write
        public static string memberListFormat =

@"public struct {0}
{{
{2}

    public void Read(ReadOnlySpan<byte> span, ref ushort count)
    {{
        {3}
    }}

    public bool Write(Span<byte> span, ref ushort count)
    {{
        bool success = true;
        {4}
        return success;
    }}

}}

public List<{0}> {1}s = new List<{0}>();";

        // {0} 변수 이름
        // {1} To~ 변수 형식
        // {2} 변수 형식
        public static string readFormat =

@"this.{0} = BitConverter.{1}(span.Slice(count, span.Length - count));
count += sizeof({2});
";

        // {0} 변수 이름
        // {1} 변수 형식
        public static string readByteFormat =

@"this.{0} = ({1})segment.Array[segment.Offset + count];
count += sizeof({1});
";

        // {0} 변수 이름
        public static string readStringFormat =

@"ushort {0}Len = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
count += sizeof(ushort);
this.{0} = Encoding.Unicode.GetString(span.Slice(count, {0}Len));
count += {0}Len;
";
// {0} 리스트 이름 [대문자]
// {1} 리스트 이름 [소문자]
public static string readListFormat =
@"{1}s.Clear();
ushort {1}Len = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
count += sizeof(ushort);
for (int i = 0; i < {1}Len; i++)
{{
    {0} {1} = new {0}();
    {1}.Read(span, ref count);
    {1}s.Add({1});
}}
";
// {0} 변수 이름
// {1} 변수 형식
public static string writeFormat =
@"success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), this.{0});
count += sizeof({1});
";

        // {0} 변수 이름
        // {1} 변수 형식
        public static string writeByteFormat =

@"segment.Array[segment.Offset + count] = (byte)this.{0};
count += sizeof({1});
";
// {0} 변수 이름
public static string writeStringFormat =
@"ushort {0}Len = (ushort)Encoding.Unicode.GetBytes(this.{0}, 0, this.{0}.Length, segment.Array, segment.Offset + count + sizeof(ushort));
success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), {0}Len);
count += sizeof(ushort);
count += {0}Len;
";

        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        public static string writeListFormat =

@"success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort){1}s.Count);
count += sizeof(ushort);
foreach ({0} {1} in this.{1}s)
success &= {1}.Write(span, ref count);
";
}

ex 2) PDL.xml

<?xml version="1.0" encoding="utf-8" ?>
<PDL>
  <packet name="PlayerInfoReq">
    <byte name="testByte"/>
    <long name="playerId"/>
    <string name="name"/>
    <list name="skill">
      <int name="id"/>
      <short name="level"/>
      <float name="duration"/>
      <list name="attribute">
        <int name="att"/>
      </list>
    </list>
  </packet>
  <packet name="Test">
    <int name="testInt"/>
  </packet>
</PDL>

ex 3) main program class
class Program
{
static string genPackets; // 실시간으로 만들어지는 패킷 코드
static ushort packetId; // 1, 2, 3, ...
static string packetEnums;
static void Main(string[] args)
{
XmlReaderSettings settings = new XmlReaderSettings() // 환경 설정
{
IgnoreComments = true, // 주석 무시
IgnoreWhitespace = true // 스페이스 바 무시
};

        using (XmlReader reader = XmlReader.Create("PDL.xml", settings))
        {
            reader.MoveToContent();      // 헤더를 건너뛰고 핵심 내용으로 바로 들어감

            while (reader.Read())        // string 방식으로 읽어드림
            {
                if (reader.Depth == 1 && reader.NodeType == XmlNodeType.Element)    // Element 시작, EndElement 끝
                {
                    ParsePacket(reader);
                }
                // System.Console.WriteLine(reader.Name + " " + reader["name"]);     // ["name"] : name이라는 콘텐츠만 읽는다.
            }
            string fileText = string.Format(PacketFormat.fileFormat, packetEnums, genPackets);
            File.WriteAllText("GenPacket.cs", fileText);    // 조립
        }

        // reader.Dispose();        // 사용을 닫아줌 or using 사용(알아서 범위 내에서 Dispose 호출)
    }

    public static void ParsePacket(XmlReader reader)
    {
        if (reader.NodeType == XmlNodeType.EndElement)  // 잘못 들어옴
        {
            return;
        }
        if (reader.Name.ToLower() != "packet")          // 소문자로 변환 후 packet이 아니면 return
        {
            System.Console.WriteLine("Invalid packet node");
            return;
        }

        string packetName = reader["name"];
        if (string.IsNullOrEmpty(packetName))       // 비어있는지
        {
            System.Console.WriteLine("Packet without name");
            return;
        }

        Tuple<string, string, string> tuple = ParseMembers(reader);
        genPackets += string.Format(PacketFormat.packetFormat, packetName, tuple.Item1, tuple.Item2, tuple.Item3);
        packetEnums += string.Format(PacketFormat.packetEnumFormat, packetName, ++packetId) + Environment.NewLine + "\t";
    }

    // {1} 멤버 변수들
    // {2} 멤버 변수 Read
    // {3} 멤버 변수 Write
    public static Tuple<string, string, string> ParseMembers(XmlReader reader)
    {
        string packetName = reader["name"];

        string memberCode = "";     // For Tuple
        string readCode = "";       // For Tuple
        string writeCode = "";      // For Tuple

        int depth = reader.Depth + 1;       // packet의 depth+1
        while (reader.Read())
        {
            if (reader.Depth != depth)
            {
                break;
            }
            string memberName = reader["name"];
            if (string.IsNullOrEmpty(memberName))
            {
                System.Console.WriteLine("Member without name");
                return null;
            }
            if (string.IsNullOrEmpty(memberCode) == false)
            {
                memberCode += Environment.NewLine;      // 내용이 있으면 개행
            }

            string memberType = reader.Name.ToLower();
            switch (memberType)
            {
                case "byte":
                case "sbyte":
                    memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                    readCode += string.Format(PacketFormat.readByteFormat, memberName, memberType);
                    writeCode += string.Format(PacketFormat.writeByteFormat, memberName, memberType);
                    break;
                case "bool":
                case "short":
                case "ushort":
                case "int":
                case "long":
                case "float":
                case "double":
                    memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                    readCode += string.Format(PacketFormat.readFormat, memberName, ToMemberType(memberType), memberType);
                    writeCode += string.Format(PacketFormat.writeFormat, memberName, memberType);
                    break;
                case "string":
                    memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                    readCode += string.Format(PacketFormat.readStringFormat, memberName);
                    writeCode += string.Format(PacketFormat.writeStringFormat, memberName);
                    break;
                case "list":
                    Tuple<string, string, string> tuple = ParseList(reader);
                    memberCode += tuple.Item1;
                    readCode += tuple.Item2;
                    writeCode += tuple.Item3;
                    break;
                default:
                    break;
            }
        }
        // 정렬을 위해서
        memberCode = memberCode.Replace("\n", "\n\t");      // ("\n"을 \"n\t"으로 바꿈)
        readCode = readCode.Replace("\n", "\n\t\t");
        writeCode = writeCode.Replace("\n", "\n\t\t");

        // 한 번에 세 개를 반환
        return new Tuple<string, string, string>(memberCode, readCode, writeCode);
    }

    public static Tuple<string, string, string> ParseList(XmlReader reader)
    {
        string listName = reader["name"];
        if (string.IsNullOrEmpty(listName))
        {
            System.Console.WriteLine("List without name");
            return null;
        }

        Tuple<string, string, string> tuple = ParseMembers(reader);

        string memberCode = string.Format(PacketFormat.memberListFormat,
            FirstCharToUpper(listName),
            FirstCharToLower(listName),
            tuple.Item1,
            tuple.Item2,
            tuple.Item3
        );

        string readCode = string.Format(PacketFormat.readListFormat,
            FirstCharToUpper(listName),
            FirstCharToLower(listName)
        );

        string writeCode = string.Format(PacketFormat.writeListFormat,
            FirstCharToUpper(listName),
            FirstCharToLower(listName)
        );

        return new Tuple<string, string, string>(memberCode, readCode, writeCode);
    }

    public static string ToMemberType(string memberType)
    {
        switch (memberType)
        {
            case "bool":
                return "ToBoolean";
            case "short":
                return "ToInt16";
            case "ushort":
                return "ToUINT16";
            case "int":
                return "ToInt32";
            case "long":
                return "ToInt64";
            case "float":
                return "ToSingle";
            case "double":
                return "ToDouble";
            default:
                return "";
        }
    }

    public static string FirstCharToUpper(string input)
    {
        if (string.IsNullOrEmpty(input)) return "";
        return input[0].ToString().ToUpper() + input.Substring(1);
    }

    public static string FirstCharToLower(string input)
    {
        if (string.IsNullOrEmpty(input)) return "";
        return input[0].ToString().ToLower() + input.Substring(1);
    }

}

- byte는 BitConverter로 할 필요가 없다.
- 2중 리스트는 패킷 생성을 자동화할 수는 있지만 struct를 class로 바꿔줘야 한다.

---

[Packet Generator #4]

ex) .csproj 파일
<PropertyGroup>
<AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
</PropertyGroup>

- /bin 폴더에 dll, exe, ... 등을 넣는다. (/netcore x.x에 넣지 않는다. build할 때는 debug폴더에)

  <PropertyGroup>
    <OutputPath>bin\Debug</OutputPath>
  </PropertyGroup>

- bin\Debug 에 실행파일들을 담는다.

* batch 파일(.bat)

- batch(.bat) 파일은 윈도우의 적용된 명령어들을 작성해서 한 번에 실행하게 해줌.
- 여기선 .exe 파일을 자동으로 실행하게 해주려고 한다.

ex ) batch 파일로 덮어씌우기(.bat 파일)
START ../../PacketGenerator/bin/Debug/PacketGenerator.exe ../../PacketGenerator/PDL.xml
XCOPY /Y GenPackets.cs "../../DummyClient/Packet"
XCOPY /Y GenPackets.cs "../../Server/Packet"

- START를 이용해 exe파일 실행하기(argument를 넘겨줌)
- XCOPY를 이용해 복사(덮어씌우기 옵션: /Y)

---

[Packet Generator #5]

- Packet Manager(Server session의 onRecieve() 함수 리팩토링)

* Packet의 Deserialize를 하는 Packet Manager 코드를 작성한다.(나중에 자동화)

ex) Pacekt Manager 클래스
class PacketManager
{
// 고정된 메모리 영역을 최초로 한 번만 할당한다.
#region Singleton  
 static PacketManager \_instance;
public static PacketManager Instance
{
get
{
if (\_instance == null) // 처음만 만든다.
\_instance = new PacketManager();
return \_instance;
}
}
#endregion

    // Event delegate
    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>> _onRecv = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> _handler = new Dictionary<ushort, Action<PacketSession, IPacket>>();

    public void Register()    // 멀티쓰레드 환경에서는 등록에 대해 락을 걸어야 한다.
    {
      _onRecv.Add((ushort)PacketID.PlayerInfoReq, MakePacket<PlayerInfoReq>);
      _handler.Add((ushort)PacketID.PlayerInfoReq, PacketHandler.PlayerInfoReqHandler);
    }

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
      // Deserialization(size, id)
      ushort count = 0;

      ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
      count += 2;
      ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
      count += 2;

      // Parsing
      Action<PacketSession, ArraySegment<byte>> action = null;
      if (_onRecv.TryGetValue(id, out action))        // dictionary에서 찾아서 handler를 등록했으면 invoke
          action.Invoke(session, buffer);

      Console.WriteLine($"RecvPacketId: {id}, Size: {size}");
    }

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T : IPacket, new()  // Generic으로 넘겨준다(IPacket일 경우 new() 해준다.)
    {
      T packet = new T();
      packet.Read(buffer);     // Deserialize

      Action<PacketSession, IPacket> action = null;
      if (_handler.TryGetValue(packet.Protocol, out action))
          action.Invoke(session, packet);
    }

}

- Packet Handler(Server session의 onRecieve() 함수 리팩토링)

* 이후 패킷이 조립되면 할 내용들을 PacketHandler 코드를 작성한다.

ex) Packet Handler 클래스, 예제에서는 로그 출력
class PacketHandler
{
public static void PlayerInfoReqHandler(PacketSession session, IPacket packet)
{
PlayerInfoReq p = packet as PlayerInfoReq; // 캐스팅

      System.Console.WriteLine($"PlayerInfoReq: {p.playerId} {p.name}");

      foreach (PlayerInfoReq.Skill skill in p.skills)
      {
          System.Console.WriteLine($"Skill: {skill.id} {skill.level} {skill.duration}");
      }
    }

}

---

[Packet Generator #6]

- 패킷 매니저 자동화

* 용도 구분

1. Server -> Server
2. Client -> Server
3. Server -> Client

- 용도를 구분하여 Packet을 정의하고 Handler도 다르게 정의해야한다.
- Pakcet Manager에서 Register()를 필요한 것만(용도 구분하여) 이벤트(Handler)를 등록한다.

* 용도 구분하기 위한 방법

1. PDL.xml 파일에서 이름 정의에 prefix(접두사)를 붙인다.(규칙을 정함)
   ex)
   C* : Client의 Packet
   S* : Server의 Packet

---

[채팅 테스트 #1, #2]

- 채팅 테스트

* 일반적인 자료구조는 멀티쓰레드 환경에서 버그가 발생한다.
* 일반적으로 MMO RPG에서 이동 패킷은 1초에 4번 보낸다.
* 멀티쓰레드 환경은 lock만을 잡으면 해결되는 것이 아니라 쓰레드들이 몰리는 점까지도 해결해줘야 한다.
* 실질적으로는 큐에 넣고 다른 일을 한다.(Job or Task)
  ex) Server의 SessionManager
  class SessionManager
  {
  static SessionManager \_session = new SessionManager();
  public static SessionManager Instance { get { return \_session; } }
  int \_sessionId = 0;
  Dictionary<int, ClientSession> \_sessions = new Dictionary<int, ClientSession>();
  object \_lock = new object();

      public ClientSession Generate()
      {
        lock (_lock)
        {
            int sessionId = ++_sessionId;

            ClientSession session = new ClientSession();
            session.SessionId = sessionId;
            _sessions.Add(sessionId, session);

            System.Console.WriteLine($"Connected : {sessionId}");

            return session;
        }
      }

      public ClientSession Find(int id)
      {
        lock (_lock)
        {
            ClientSession session = null;
            _sessions.TryGetValue(id, out session);
            return session;
        }
      }

      public void Remove(ClientSession session)
      {
        lock (_lock)
        {
            _sessions.Remove(session.SessionId);
        }
      }

  }

* SessionManager를 통해서 Session을 생성하고 관리한다.

ex) Client의 SessionManager
class SessionManager
{
static SessionManager \_session = new SessionManager();
public static SessionManager Instance { get { return \_session; } }

    List<ServerSession> _sessions = new List<ServerSession>();
    object _lock = new object();

    public ServerSession Generate()
    {
      lock (_lock)
      {
        ServerSession session = new ServerSession();
        _sessions.Add(session);
        return session;
      }
    }

    public void SendForEach()
    {
      lock (_lock)
      {
        foreach (ServerSession session in _sessions)
        {
          C_Chat chatPacket = new C_Chat();
          chatPacket.chat = $"Hello Server !";
          ArraySegment<byte> segment = chatPacket.Write();

          session.Send(segment);
        }
      }
    }

}

- Client의 Session을 관리한다.(생성하고 지우는 역할)

ex) 채팅을 할 GameRoom
class GameRoom
{
List<ClientSession> \_sessions = new List<ClientSession>();
object \_lock = new object();
public void BroadCast(ClientSession session, string chat)
{
S_Chat packet = new S_Chat();
packet.playerId = session.SessionId;
packet.chat = $"{chat} I am {packet.playerId}";
ArraySegment<byte> segment = packet.Write();

      lock (_lock)
      {
        foreach (ClientSession s in _sessions)
            s.Send(segment);
      }
    }
    public void Enter(ClientSession session)
    {
      lock (_lock)
      {
        _sessions.Add(session);
        session.Room = this;    // 자신을 Room에 연결
      }
    }

    public void Leave(ClientSession session)
    {
      lock (_lock)
      {
        _sessions.Remove(session);
      }
    }

}

- BroadCast 부분에서 쓰레드가 계속 몰린다.
- 병목현상: 쓰레드가 계속 몰리면서 새로운 쓰레드를 생성하고 교착상태는 아니지만 작업들이 밀리는 상태가 발생한다.(렉의 느낌)
- 큐를 사용하여 해결한다.(Job or Task)

---

[Command 패턴]

- Command 패턴

* 요청이 들어오면 바로 처리하는 것이 아니라 큐에 넣고 처리한다.
* 식당의 예시에서는 주문을 받으면 주문서에 적고(캡슐화 하고) 처리한다.
* 주문을 받는 것 + 주문을 넣는 것으로 나누어 처리한다.

---

[JobQueue #1, #2]

ex) JobQueue 클래스와 인터페이스(Action을 Push)
public interface IJobQueue
{
void Push(Action job);
}

public class JobQueue : IJobQueue
{
Queue<Action> \_jobQueue = new Queue<Action>();
object \_lock = new object();
bool \_flush = false; // Queue에 쌓인 것을 실행할건지 말건지 정하는 변수

    public void Push(Action job)
    {
      bool flush = false;

      lock (_lock)
      {
        _jobQueue.Enqueue(job);
        if (_flush == false)
            flush = _flush = true;
      }

      if (flush)
        Flush();        // 순차적으로 하나의 쓰레드만 처리
    }
    void Flush()
    {
      while (true)
      {
        Action action = Pop();
        if (action == null)
            return;

        action.Invoke();
      }
    }
    Action Pop()
    {
      lock (_lock)
      {
        if (_jobQueue.Count == 0)
        {
          _flush = false;
          return null;
        }
        return _jobQueue.Dequeue();
      }
    }

}

- lock을 걸어서 하나의 쓰레드씩 처리될 수 있도록 한다.
- Push()를 인터페이스로 작성하여 각기 다른 일을 Push()할 수 있도록 한다.(주문서에는 여러 가지 음식이 있는 것과 비슷하다)

- 만약 Room이 있을 경우는 다음과 같이 참조 형태로 null 처리의 크래시 발생을 방지한다.
  ex) push()에 대한 것은 참조 변수로 호출
  GameRoom room = Room; // Room이 null로 되어 크래시 발생을 방지
  room.Push(
  () => room.Leave(this)
  );
  Room = null;

ex) TaskQueue 클래스와 인터페이스 --- 처리할 일들을 정의.
interface ITask
{
void Execute();
}

class BroadcastTask : ITask
{
GameRoom \_room;
ClientSession \_session;
string \_chat;

    BroadcastTask(GameRoom room, ClientSession session, string chat)
    {
      _room = room;
      _session = session;
      _chat = chat;
    }
    public void Execute()
    {
      _room.BroadCast(_session, _chat);
    }

}

class TaskQueue
{
Queue<ITask> \_queue = new Queue<ITask>();
}

- 람다 문법이 생기기 전에는 클래스로 정의하여 일을 정의하였다.
- 실무 프로젝트에서는 람다 : 클래스 정의 = 7 : 3 비율로 쓰인다.

---

[패킷 모아보내기]

- 패킷 모아보내기

* 서버 스레드의 병목 현상을 감해준다. 1)엔진단, 2)콘텐츠단 둘 다 구현

ex) pending List를 만들어 계속 모으고 한꺼번에 Broadcast를 한다.
List<ArraySegment<byte>> \_pendingList = new List<ArraySegment<byte>>();

public void BroadCast(ClientSession session, string chat)
{
S_Chat packet = new S_Chat();
packet.playerId = session.SessionId;
packet.chat = $"{chat} I am {packet.playerId}";
ArraySegment<byte> segment = packet.Write();

      _pendingList.Add(segment);     // 바로 뿌리는 것이 아니라 pending 시킨다.

}

- 실제로는 AI의 행동도 같이 List에 넣고 보낸다.

ex) Send()의 overloading으로 Server의 pending List 처리
// Overloading
public void Send(List<ArraySegment<byte>> sendBuffList) // 언제 할 지 예측 불가
{
if (sendBuffList.Count == 0) // 예외 처리
return;

      lock (_lock)    // 한 번에 한 쓰레드만 들어올 수 있게 한다.
      {
          foreach (ArraySegment<byte> sendBuff in sendBuffList)
              _sendQueue.Enqueue(sendBuff);       // Queue에만 넣고 스킵할 수도 있다.

          if (_pendingList.Count == 0)  // 쓰레드 1빠로 send() 호출(전송까지 할 수 있다)
          {
              RegisterSend();
          }
      }

}

---

[JobTimer]

- Server의 추가적인 일

* log를 파일로 기록
* crash가 났을 경우, crash dump를 기록

- 중앙 관리 시스템(메인 쓰레드)

1. Tick으로 여러 일 처리

- 시작할 시간이 아니더라도 if 문을 전부 돌아야 하기 때문에 비효율
  ex)
  int roomTick = 0;
  // ...Tick
  // ...Tick
  // ...Tick
  // ...Tick
  // ...Tick
  // ...Tick
  while (true)
  {
  int now = System.Environment.TickCount;
  if (roomTick < now)
  {
  Room.Push(() => Room.Flush());
  roomTick = now + 250;
  }
  // TODO

        // TODO

        // TODO

  }

2. 예약 시스템

- 우선 순위 큐로 구현
  ex) Priority Queue template
  public class PriorityQueue<T> where T : IComparable<T>
  {
  List<T> \_heap = new List<T>();
  public int Count { get { return \_heap.Count; } }

      // O(logN)
      public void Push(T data)
      {
        // 힙의 맨 끝에 새로운 데이터를 삽입
        _heap.Add(data);

        int now = _heap.Count - 1;

        // 도장 깨기를 시작
        while (now > 0)
        {
          // 도장 깨기를 시도
          int next = (now - 1) / 2;
          if (_heap[now].CompareTo(_heap[next]) < 0)
            break;  // 실패

          // 두 값을 교체
          T temp = _heap[now];
          _heap[now] = _heap[next];
          _heap[next] = temp;

          // 검사 위치를 이동
          now = next;
        }
      }
      // O(logN)
      public T Pop()
      {
        // 반환할 데이터를 따로 저장
        T ret = _heap[0];

        // 마지막 데이터를 루트로 이동
        int lastIndex = _heap.Count - 1;
        _heap[0] = _heap[lastIndex];
        _heap.RemoveAt(lastIndex);
        lastIndex--;

        // 역으로 내려가는 도장깨기 시작
        int now = 0;
        while (true)
        {
          int left = 2 * now + 1;
          int right = 2 * now + 2;

          int next = now;

          // 왼쪽 값이 현재 값보다 크면, 왼쪽으로 이동
          if (left <= lastIndex && _heap[next].CompareTo(_heap[left]) < 0)
            next = left;

          // 오른쪽 값이 현재 값(왼쪽 이동 포함)보다 크면, 오른쪽으로 이동
          if (right <= lastIndex && _heap[next].CompareTo(_heap[right]) < 0)
            next = right;

          // 왼쪽, 오른쪽 모두 현재 값보다 작으면 종료
          if (next == now)
            break;

          // 두 값을 교체
          T temp = _heap[now];
          _heap[now] = _heap[next];
          _heap[next] = temp;

          // 검사 위치를 이동
          now = next;
        }
        return ret;
      }

      public T Peek()     // 조회
      {
        if (_heap.Count == 0)
          return default(T);
        return _heap[0];        // 가장 좋은 value
      }

  }

ex) JobTimer class 구현(예약 시스템)
struct JobTimerElem : IComparable<JobTimerElem> // job 구조체
{
public int execTick; // 실행 시간
public Action action; // 할 Action

    public int CompareTo(JobTimerElem other)
    {
      return other.execTick - execTick;       // Tick이 작은 것이 먼저
    }

}
class JobTimer
{
PriorityQueue<JobTimerElem> \_pq = new PriorityQueue<JobTimerElem>();
object \_lock = new object();

    public static JobTimer Instance { get; } = new JobTimer();

    public void Push(Action action, int tickAfter = 0)  // 몇 tick 후에 action을 시작
    {
      JobTimerElem job;
      job.execTick = System.Environment.TickCount + tickAfter;    // 현재 시간 + 몇 tick 후에
      job.action = action;        // Action 시작

      lock (_lock)
      {
        _pq.Push(job);
      }
    }

    public void Flush()
    {
      while (true)
      {
        int now = System.Environment.TickCount;

        JobTimerElem job;

        lock (_lock)
        {
          if (_pq.Count == 0)
            break;          // while문을 나감

          job = _pq.Peek();       // top() 조회
          if (job.execTick > now)     // 아직이다.
            break;

          _pq.Pop();
        }

        job.action.Invoke();    // 일감을 실행 명령
      }
    }

}

ex) MainThread 역할
static void FlushRoom() // 일감
{
Room.Push(() => Room.Flush());
JobTimer.Instance.Push(FlushRoom, 250); // 0.25초마다 하도록 예약
}

JobTimer.Instance.Push(FlushRoom); // 예약
while (true)
{
JobTimer.Instance.Flush();
}

3. [20ms][20ms][...][...]...[]: list로 관리하여 더 효율적인 예약

- (...)안에는 연결 리스트로 돼있다.

---

[유니티 연동 #1]

- 유니티 제약

* Unity는 Thread가 1 개이므로 Background에서 Thread를 돌리고 Unity object에는 관여할 수 없다.
* DLL을 가져와서 사용할 수 있지만 Debug의 어려움으로 같이 Build하는 방법으로 권장함.
* span 사용 불가 -> Array.Copy(byte[], ...)로 구성한다.

---

[유니티 연동 #2]

- Unity의 정책

* Unity의 정책에서는 다른 쓰레드로 접근하는 것을 막았다.
* Unity는 메인 쓰레드만 사용한다, 다른 Thread로 Unity Component를 사용할 수 없다(전달은 가능)
* 외부에 PacketQueue.cs를 작성하여 Queue를 Pop할 때마다 Handler를 이용할 수 있도록 한다.

- PacketQueue

* Unity의 정책(only 단일 쓰레드)으로 인해 외부 스크립트에 작성하여 사용한다.
* Unity의 Component를 건드려서는 안된다.

- Func vs Action

* Func: 특정 기능을 수행하면서 return 값을 받을 때 사용, out result를 받는다 = return 값이 있다.
* Action: 특정 기능을 수행할 때 사용
* Predicate는 Func의 일종으로 bool 값을 return 한다

ex) [PacketManager.cs]
Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IPacket>> \_makeFunc = new Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IPacket>>();

public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer, Action<PacketSession, IPacket> onRecvCallback = null)
{
// Deserialization(size, id)
ushort count = 0;

    ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
    count += 2;
    ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
    count += 2;

    // Parsing
    Func<PacketSession, ArraySegment<byte>, IPacket> func = null;
    if (_makeFunc.TryGetValue(id, out func))
    {
      // dictionary에서 찾아서 handler를 등록했으면 invoke
      IPacket packet = func.Invoke(session, buffer);
      if (onRecvCallback != null)
        onRecvCallback.Invoke(session, packet);     // Packet Queue(다른 옵션이 있으면)
      else
        HandlePacket(session, packet);      // Default
    }

}

T MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T : IPacket, new() // Generic으로 넘겨준다.
{
T packet = new T();
packet.Read(buffer); // Deserialize

    return packet;

}

- Handler에 대한 수정: OnRecvPacket에 3번 째 매개 변수로 옵션을 주어 invoke 실행 시킨다.
- Func의 리턴 값을 이용

ex) [NetworkManager.cs]
void Update()
{
// 1 Frame에 1 Packet
IPacket packet = PacketQueue.Instance.Pop();
if (packet != null)
{
PacketManager.Instance.HandlePacket(\_session, packet);
}
}

IEnumerator CoSendPacket()
{
while (true)
{
yield return new WaitForSeconds(3.0f);

      // 3초 후
      C_Chat chatPacket = new C_Chat();
      chatPacket.chat = "Hello Unity !";
      ArraySegment<byte> segment = chatPacket.Write();

      _session.Send(segment);
    }

}

- Coroutine을 사용하여 Start()에서 실행하고 3초 후 패킷을 보내는 작업을 비동기화 시킨다.
- 시간 지연은 Coroutine을 활용하는 것이 좋다.

---

[유니티 연동 #3]

- 유니티 연동(방 입장 & 이동)

1. PDL.xml 코드 작성(패킷 자동화)
2. 방 입장 BroadCast 패킷: PlayerId(누가), 디폴트 위치 부여(float x, y, z) ---- Server의 패킷, 각자의 Client가 처리(Handler)
3. 사람 목록 패킷: PlayerList{ PlayerId, isSelf, 위치} ---- Seㄱver의 패킷, 각자의 Client가 처리(Handler)

- isSelf는 List에 내가 포함돼있으므로 나를 판별할 변수
- 경우에 따라서, (isSelf)boolean 방식이 아닌 독립적인 패킷으로 정의해도 된다.

4. 방 퇴장 패킷: 내용 X ---- Client의 패킷, Server가 처리(Handler)
5. 방 퇴장 Broadcast 패킷: PlayerId(누가) ---- Server의 패킷, Client가 처리(Handler)

6. 이동 패킷: posX, posY, posZ ---- Client의 패킷, Server가 처리(Handler)

- Rotation 패킷도 필요하다(자연스러움)
- 자신의 PlayerId를 보내지 않는 이유는 ClientSession에 포함돼있기 때문이다.
- ClientSession에 있는 정보를 분할해야 좋다.(ClientSession은 Client 정보만)
- Unity의 이동 패킷: 자신의 목적지의 위치를 보내준다, 정의 방법 2가지.
  a. Primitive type인 float posX, posY, posZ를 정의
  b. Vector3를 정의한다(Vector3에 대한 자동화 코드 작성 필수!)

7. 이동 Broadcast 패킷: PlayerId(누가), 위치(x, y, z) ---- Server의 패킷, 각자의 Client가 처리(Handler)

ex) [PDL.xml]

  <?xml version="1.0" encoding="utf-8" ?>
  <PDL>
    <packet name="S_BroadcastEnterGame">      // 2)
      <int name="playerId"/>
      <float name="posX"/>
      <float name="posY"/>
      <float name="posZ"/>
    </packet>
    <packet name="C_LeaveGame">             // 4)
    </packet>
    <packet name="S_BroadcastLeaveGame">      // 5)
      <int name="playerId"/>
    </packet>
    <packet name="S_PlayerList">          // 3)
      <list name="player">
        <bool name="isSelf"/>
        <int name="playerId"/>
        <float name="posX"/>
        <float name="posY"/>
        <float name="posZ"/>
      </list>
    </packet>
    <packet name="C_Move">      // 6)
      <float name="posX"/>
      <float name="posY"/>
      <float name="posZ"/>
    </packet>
    <packet name="S_BroadcastMove">     // 7)
      <int name="playerId"/>
      <float name="posX"/>
      <float name="posY"/>
      <float name="posZ"/>
    </packet>
  </PDL>

---

[유니티 연동 #4]

- Unity Player 이동 동기화

1.  Player(PlayerId 포함), MyPlayer(Player를 상속, 내 움직임 관련) script를 작성 --- 나중에는 PlayerManager로 방에 입장하는 Player들을 추가한다.
    ex) [Player.cs]
    public class Player : MonoBehaviour
    {
    public int PlayerId { get; set; }
    }
    ex) [MyPlayer.cs]
    public class MyPlayer : Player
    {
    NetworkManager \_network;
    // Start is called before the first frame update
    void Start()
    {
    \_network = GameObject.Find("NetworkManager").GetComponent<NetworkManager>(); // NetworkManager 오브젝트의 Script 연결
    StartCoroutine("CoSendPacket");
    }

        IEnumerator CoSendPacket()
        {
          while (true)
          {
            // 이동 패킷은 1초에 4번
            yield return new WaitForSeconds(0.25f);

            // 0.25초 후
            C_Move movePacket = new C_Move();
            movePacket.posX = UnityEngine.Random.Range(-50, 50);    // 랜덤값
            movePacket.posY = 0;    // 높이는 0
            movePacket.posZ = UnityEngine.Random.Range(-50, 50);    // 랜덤값

            _network.Send(movePacket.Write());      // ArraySegment를 보내줌
          }
        }

    }

- 이동 패킷은 1초에 4번 정도가 평균이다.
- MyPlayer의 나의 움직임은 NetworkManager에서 send()를 한다

2. Player, MyPlayer를 관리하는 PlayerManager를 만든다.

- Monobehaviour를 상속X, 데이터만 들고있음(컴포넌트 기생X)
- 컴포넌트로 기생하는 것은 Monobehaviour를 상속받는다.

ex) [PlayerManager.cs]
public class PlayerManager
{
MyPlayer \_myPlayer;
Dictionary<int, Player> \_players = new Dictionary<int, Player>();

    public static PlayerManager Instance { get; } = new PlayerManager();   // 어디서든 접근할 수 있도록

    public void Add(S_PlayerList packet)
    {
      Object obj = Resources.Load("Player");

      foreach (S_PlayerList.Player p in packet.players)
      {
        GameObject go = Object.Instantiate(obj) as GameObject;
        if (p.isSelf)    // 내가 관리
        {
          MyPlayer myPlayer = go.AddComponent<MyPlayer>();        // MyPlayer Script 첨가
          myPlayer.PlayerId = p.playerId;
          myPlayer.transform.position = new Vector3(p.posX, p.posY, p.posZ);
          _myPlayer = myPlayer;
        }
        else    // 남들이 관리
        {
          Player player = go.AddComponent<Player>();      // Player Script 첨가
          player.transform.position = new Vector3(p.posX, p.posY, p.posZ);
          player.PlayerId = p.playerId;
          _players.Add(p.playerId, player);
        }
      }
    }
    public void Move(S_BroadcastMove packet)
    {
      // 서버 응답이 와서 움직일 수 있다.
      if (_myPlayer.PlayerId == packet.playerId)
      {
        _myPlayer.transform.position = new Vector3(packet.posX, packet.posY, packet.posZ);
      }
      else
      {
        Player player = null;
        if (_players.TryGetValue(packet.playerId, out player))
        {
          // PlayerId를 찾았다.
          player.transform.position = new Vector3(packet.posX, packet.posY, packet.posZ);
        }
      }
    }
    public void EnterGame(S_BroadcastEnterGame packet)
    {
      if (packet.playerId == _myPlayer.PlayerId)
        return;

      Object obj = Resources.Load("Player");
      GameObject go = Object.Instantiate(obj) as GameObject;

      Player player = go.AddComponent<Player>();      // Player Script 첨가
      player.transform.position = new Vector3(packet.posX, packet.posY, packet.posZ);
      _players.Add(packet.playerId, player);
    }
    public void LeaveGame(S_BroadcastLeaveGame packet)
    {
      if (_myPlayer.PlayerId == packet.playerId)
      {
        GameObject.Destroy(_myPlayer.gameObject);
        _myPlayer = null;
      }
      else
      {
        Player player = null;
        if (_players.TryGetValue(packet.playerId, out player))
        {
          // PlayerId를 찾았다.
          GameObject.Destroy(player.gameObject);
          _players.Remove(packet.playerId);
        }
      }
    }

}

- PlayerList의 Player를 이동시킬 때, 자신도 이동시키는 것을 유의(Handler)
- 순간적인 이동에 대한 것은 부드럽게 lerp로 이동시킨다.
- Handler에서는 함수들을 호출하는 식의 코드를 작성한다.(MonoBehaviour 상속 X)
- Packet Queue는 Broadcast 가능하도록 PopAll()을 구현한다.

ex) PopAll() 함수
public List<IPacket> PopAll()
{
List<IPacket> list = new List<IPacket>();

    lock (_lock)
    {
      while (_packetQueue.Count > 0)
        list.Add(_packetQueue.Dequeue());
    }

    return list;

}

3. 이동 동기화(2가지 방법)
   a. 서버에서 허락 패킷이 왔을 때만 이동 가능 --- ex) LOL
   b. 클라이언트에서 이동하고 있다가 서버에서 응답이 오면 보간 --- ex) 배틀그라운드 등의 FPS 게임

---