## MRect class

MRect 클래스는 뷰에 컨트롤을 동적으로 배치하기 위한 클래스이다.

생성자로 **위치 상태**와 4개의 거리변수를 입력받는다.

4개의 변수는 위치 상태에 따라 다른 값을 가지게 된다.


```cpp
@function
template<class T> T CreateControl(CWnd* pWnd, MRect base) {
	return T(new T::element_type(pWnd, base));
}
```
```cpp
@MRect constructor
MRect(MRectPosition, xd, yd, xd2, yd2));
```

### MRectPosition::L `좌측면을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/xaFHhCo.png" width="640">

### MRectPosition::R `우측면을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/FQj3D83.png" width="640">

### MRectPosition::T `상단면을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/nPOOw86.png" width="640">

### MRectPosition::B `하단면을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/96uKx3w.png" width="640">

* * *

### MRectPosition::LT `좌측 상단을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/ICjHZRL.png" width="640">

### MRectPosition::RT `우측 상단을 기준으로 위치가 결정된다.`
<img src="https://i.imgur.com/t6WcUqI.png" width="640">

### MRectPosition::LB `우측 하단을 기준으로 위치가 결정된다.`
### MRectPosition::RB `좌측 하단을 기준으로 위치가 결정된다.`

모서리를 중심으로 컨트롤을 배치할 땐, xd와 yd는 margin이 되고 xd2와 yd2는 width와 height가 된다.

### MRectPosition::DYNAMIC `모든 면에 대해 동적인 크기를 가진다.`
<img src="https://i.imgur.com/NW9PDlv.png" width="640">

