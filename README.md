## MSpring
#### Header only MFC custom design library.

### 1. MSpring 준비 하기

#### 1-가. MFC 프로젝트 생성

![](https://i.imgur.com/CbAwegE.png)
Visual Studio 에서 새 프로젝트를 추가합니다. **Visual C++** -> **MFC** 를 선택 한 후, **MFC 응용 프로그램** 을 선택 합니다.

![](https://i.imgur.com/YUPFPdc.png)
위와 같이 프로젝트를 설정합니다. 반드시 **단일 문서** 를 선택하고 , 문서/뷰 아키텍처 지원을 해제 해야 합니다.

**두꺼운 프레임** 만을 제외하고 모든 속성은 해제 시킵니다.


#### 1-나. MSpring 준비.

MSpring은 헤더파일 라이브러리 이므로, include 디렉터리 또는 프로젝트 디렉터리에 폴더를 놓기만 하면 됩니다.

![](https://i.imgur.com/YPl8bdy.png)

위 예시 처럼 프로젝트 디렉터리에서 아래의 명령으로 라이브러리를 다운로드 합니다.

```bash
git clone https://github.com/springkim/MSpring
```

#### 1-다. MSpring Frame 설정.

* `MainFrm.h` 에서 `#include"MSpring/MSpring.h"` 를 추가합니다.

* `MainFrm.h` 와 `MainFrm.cpp` 에서 `CFrameWnd` 를 모두 `MSpringFrame` 으로 변경합니다.

* `MainFrm.cpp` 의 `int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)` 함수의 밑에 아래의 코드를 첨부 합니다.

```cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MSpringFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	this->AddSysBtn(RESOURCE_CLOSE, MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(RESOURCE_MAXIMIZE, MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(RESOURCE_MINIMIZE, MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);
	this->SetStyle(TEXT("Arial"), RGB(255, 255, 255), RGB(33, 33, 33), RGB(255, 0, 0));
	return 0;
}
```



#### 1-라. MSpring View 설정.

`ChildView.h` 와 `ChildView.cpp` 를 삭제 합니다. 이 파일들은 사용하지 않습니다.

`MainFrm.h` 의 멤버 변수로 선언된 **m_wndView** 의 자료형을 **MSpringView** 로 변경 합니다.

빌드하면 커스텀된 프레임과 뷰를 볼 수 있습니다.

* * *



MFC library



###version 0.0.2
* Fixed close button problem.
* Removed frame moving bottleneck.

###version 0.0.3
* Append version information.

###version 0.0.4
* Fixed minimized window error.