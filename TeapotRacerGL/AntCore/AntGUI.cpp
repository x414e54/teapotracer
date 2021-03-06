#include "AntGUI.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void AntGUIElement::SetTexture(UINT id, RECT textureRect)
{
	_textureId=id;
	_textureRect=textureRect;
}

void AntGUIElement::SetFont(UINT id, AntFontColorARGB fontColor)
{
	_fontId=id;
	_fontColor=fontColor;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIDialog::AntGUIDialog() 
{
	RECT defaultRect = {0,0,0,0};
	_boundingBox=defaultRect; 
	_x=0;
	_y=0;
	_h=0;
	_w=0;
	_enabled=true;
	_visible=false;
	_id=0;
}

AntGUIDialog::~AntGUIDialog()
{
	for (unsigned int i=0; i<_components.size(); i++)
	{
		delete _components.at(i);
	}
	_components.clear();
}

void AntGUIDialog::Init(AntRenderer* renderer)
{
	_renderer=renderer;
	_renderer->AddFont(L"MS Sans Serif",8,12);
	_renderer->AddFont(L"Arial",8,12);
}

void AntGUIDialog::AddImageText(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUIImageText* component = new AntGUIImageText(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddButton(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUIButton* component = new AntGUIButton(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddComboBox(int id, int x, int y, int h, int w)
{
	AntGUIComboBox* component = new AntGUIComboBox(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
}

void AntGUIDialog::AddCheckBox(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUICheckBox* component = new AntGUICheckBox(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddTextBox(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUITextBox* component = new AntGUITextBox(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddTextArea(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUITextArea* component = new AntGUITextArea(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddListBox(int id, const std::wstring& text, int x, int y, int h, int w)
{
	AntGUIListBox* component = new AntGUIListBox(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetText(text);
}

void AntGUIDialog::AddSlider(int id, int x, int y, int h, int w, int min, int max, int value)
{
	AntGUISlider* component = new AntGUISlider(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetMin(min);
	component->SetMax(max);
	component->SetValue(value);
}

void AntGUIDialog::AddProgressBar(int id, int x, int y, int h, int w, int min, int max, int value)
{
	AntGUIProgressBar* component = new AntGUIProgressBar(this);
	
	AddComponent(component);

	component->SetId(id);
	component->SetPos(x,y);
	component->SetSize(h,w);
	component->SetMin(min);
	component->SetMax(max);
	component->SetValue(value);
}

AntGUIComponent* AntGUIDialog::GetComponent(int id)
{
	for (unsigned int i=0; i<_components.size(); i++)
	{
		if (_components.at(i)!=NULL && _components.at(i)->_id==id) return _components.at(i);
	}
	return NULL;
}

AntGUIComponent* AntGUIDialog::GetComponent(int id, ANTGUI_COMPONENT_TYPE type)
{
	for (unsigned int i=0; i<_components.size(); i++)
	{
		if (_components.at(i)!=NULL && _components.at(i)->_id==id && _components.at(i)->_type==type) return _components.at(i);
	}
	return NULL;
}

AntGUIComponent* AntGUIDialog::GetIntersect(int x, int y)
{
	for (unsigned int i=0; i<_components.size(); i++) 
	{
		AntGUIComponent* component= _components.at(i);
		RECT rect = component->GetBoundingBox();
		if (x>=rect.left && x<= rect.right && y>=rect.top && y<=rect.bottom) {
			return component;
		}
	}
	return NULL;
}
void AntGUIDialog::AddComponent(AntGUIComponent* component)
{
	_components.push_back(component);
}

bool AntGUIDialog::Event( UINT uMsg, UINT p1, UINT p2)
{
	if (!_visible || !_enabled) { return false; }
	switch( uMsg )
    {
		case ANTGUI_EVENT_KEYDOWN :
			if (_focus!=NULL) { _focus->Event(uMsg, p1, p2); return false; }
		break;
		case ANTGUI_EVENT_LBUTTONUP:
			if (_focus!=NULL) { _focus->Event(uMsg, p1, p2); return false; }
		break;
		case ANTGUI_EVENT_LBUTTONDOWN:
		case ANTGUI_EVENT_MOUSEMOVE:
                POINT mouse = { short(LOWORD(p2)), short(HIWORD(p2)) };
                mouse.x += 4;
                mouse.y += 4;
				AntGUIComponent* intersect=GetIntersect(mouse.x,mouse.y);
				if (uMsg == ANTGUI_EVENT_LBUTTONDOWN) {
			  		fprintf(stderr, "Mouse click resviced at %d, %d\n",mouse.x, mouse.y);
					if (intersect!=NULL) { _focus=intersect; _focus->Event(uMsg, p1, p2); }
					else { _focus=NULL; }
					return false;
				}

				if (uMsg == ANTGUI_EVENT_MOUSEMOVE) {
					if (_mouseOver!=NULL && intersect==NULL) {_mouseOver->MouseExit(); _mouseOver=NULL; } 
					else if (_mouseOver==NULL  && intersect!=NULL) { _mouseOver=intersect; _mouseOver->MouseEnter(); } 
					else if (intersect!=NULL && intersect==_focus)	{ _focus->Event(uMsg, p1, p2); }
				}
		break;
	}

	return false;
}

void AntGUIDialog::Render(float time)
{
	if (!_visible) { return; }

	for (unsigned int i=0; i<_components.size(); i++) 
	{
		_components.at(i)->Render(time);
	}
}

void AntGUIDialog::DrawText(UINT fontId, const std::wstring& string, AntFontColorARGB* fontColor, RECT* dst)
{
	OffsetRect(dst, _x, _y);
	//int h=_renderer->GetFont(fontId)->h;
	//int w=_renderer->GetFont(fontId)->w;
	//dst->left+=(Height(dst)-h)/2;
	//dst->top+=(Height(dst)-h)/2;

	//_manager->GetFont(fontId)->dxfont->DrawText(_manager->GetSprite().dxsprite, string, -1, &dst, 0, D3DCOLOR_ARGB(fontColor._A, fontColor._R, fontColor._G, fontColor._B));
	_renderer->DrawText(fontId, string, dst, fontColor);
}

void AntGUIDialog::DrawSprite(UINT textureId, RECT* src, RECT* dst)
{
    OffsetRect(dst, _x, _y);

	POVector3 pos((float)dst->left, (float)dst->top, 0.0f);
	//_manager->GetSprite().dxsprite->Draw(_manager->GetTexture(textureId)->dxTexture, &src, NULL, &pos, 0xFFFFFFFF);
	_renderer->DrawSprite(textureId, src, &pos);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIComponent::AntGUIComponent()
{
	_mouseOver=false;
	_eventFunction=NULL;
}

void AntGUIComponent::UpdateBoundingBox()
{
	_boundingBox.top=_y;
	_boundingBox.left=_x;
	_boundingBox.bottom=_y+_h;
	_boundingBox.right=_x+_w;
}

void AntGUIComponent::EventCall(void *vParam)
{
	if (_eventFunction!=NULL)
	{
		_eventFunction(vParam);
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIImageText::AntGUIImageText(AntGUIDialog* parent)
{
	_type=ANTGUI_COMPONENT_IMAGETEXT;

	AntGUIElement elem;
	elem.SetFont(0,AntFontColorARGB(255,255,0,0));
	_elements.push_back(elem);
	_parent=parent;
}
	
void AntGUIImageText::SetText(const std::wstring& text)
{
	this->_text = text;
}

void AntGUIImageText::Render(float time)
{
	_parent->DrawText(_elements.at(0).GetFont(), _text, _elements.at(0).GetFontColor(), &_boundingBox);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIButton::AntGUIButton(AntGUIDialog* parent)
{
	_type=ANTGUI_COMPONENT_BUTTON;

	AntGUIElement elem;
	RECT rct;

	elem.SetFont(0,AntFontColorARGB(255,0,0,0));						// Button up
	//SetRect(&rct,0,0,64,19);
	SetRect(&rct,64,0,128,19);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,180,180,180));						// Mouse Over
	SetRect(&rct,64,0,128,19);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						// Button down
	SetRect(&rct,128,0,192,19);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	_parent=parent;
	_pressed=false;
	_mouseOver=false;
}

void AntGUIButton::Render(float time)
{
	if (_pressed)
	{
		_parent->DrawSprite(_elements.at(3).GetTexture(), _elements.at(3).GetTextureRect(), &_boundingBox);
		_parent->DrawText(_elements.at(3).GetFont(), _text, _elements.at(3).GetFontColor(), &_boundingBox);
	} else if (_mouseOver)
	{
		_parent->DrawSprite(_elements.at(2).GetTexture(), _elements.at(2).GetTextureRect(), &_boundingBox);
		_parent->DrawText(_elements.at(2).GetFont(), _text, _elements.at(2).GetFontColor(), &_boundingBox);
	} else
	{
		_parent->DrawSprite(_elements.at(1).GetTexture(), _elements.at(1).GetTextureRect(), &_boundingBox);
		_parent->DrawText(_elements.at(1).GetFont(), _text,  _elements.at(1).GetFontColor(), &_boundingBox);
	}
}

bool AntGUIButton::Event( UINT uMsg, UINT p1, UINT p2 )
{ 
	switch (uMsg)
	{
	case ANTGUI_EVENT_KEYDOWN :
		if (p1==ANTGUI_EVENT_KEY_RETURN) { _pressed=true; }
	break;
	case ANTGUI_EVENT_LBUTTONDOWN :
		_pressed = true;
		return true;
	break;
	case ANTGUI_EVENT_LBUTTONUP :
		_pressed = false;
		EventCall(NULL);
		return true;
	break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIComboBox::AntGUIComboBox(AntGUIDialog* parent)
{
	_type=ANTGUI_COMPONENT_COMBOBOX;

	AntGUIElement elem;
	RECT rct;

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	Background
	SetRect(&rct,64,19,127,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	ButtonUp
	SetRect(&rct,128,19,145,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	ButtonOver
	SetRect(&rct,145,19,162,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	ButtonDown
	SetRect(&rct,162,19,179,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	OpenTop
	SetRect(&rct,0,19,64,20);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	OpenMiddle
	SetRect(&rct,0,20,64,37);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	_parent=parent;
	_selected=NULL;
	_opened=false;
}

void AntGUIComboBox::AddOption(int id, const std::wstring& text, void** userData)
{
	AntGUIComboBoxOption option;
	this->_text = text;
	option._id=id;
	option._userData=userData;
	_options.push_back(option);
}

void AntGUIComboBox::SetSelected(int id)
{
	for (unsigned int i=0; i<_options.size(); i++)
	{
		if (_options.at(i)._id==id)
		{
			_selected=&_options.at(i);
		}
	}
}

void AntGUIComboBox::Render(float time)
{
	RECT frameRect = {_x,_y,_x+_w,_y+_h};
	RECT buttonRect = {_x+_w,_y,_x+_w+Width(_elements.at(2).GetTextureRect()),_y+_h};
	_parent->DrawSprite(_elements.at(1).GetTexture(), _elements.at(1).GetTextureRect(), &frameRect);
	if (_selected!=NULL)
	{
		_parent->DrawText(_elements.at(1).GetFont(), _selected->_text, _elements.at(1).GetFontColor(), &frameRect);
	} else 
	{
		_parent->DrawText(_elements.at(1).GetFont(), L"", _elements.at(1).GetFontColor(), &frameRect);
	}
	if (_opened)
	{
		RECT optionRect = {_x,_y,_x+Width(_elements.at(6).GetTextureRect()),_y+Height(_elements.at(6).GetTextureRect())};
		for (unsigned int i=0; i<_options.size(); i++)
		{
			OffsetRect(&optionRect,0,-Height(_elements.at(6).GetTextureRect()));
			if (&_options.at(i)!=_hover) {
				_parent->DrawSprite(_elements.at(6).GetTexture(), _elements.at(6).GetTextureRect(), &optionRect);
			}
			_parent->DrawText(_elements.at(6).GetFont(), _options.at(i)._text, _elements.at(6).GetFontColor(), &optionRect);
		}
		RECT topRect = {_x,_y,_x+Width(_elements.at(5).GetTextureRect()),_y+Height(_elements.at(5).GetTextureRect())};
		OffsetRect(&topRect,0,-((int)_options.size()*Height(_elements.at(6).GetTextureRect()))-Height(_elements.at(5).GetTextureRect()));
		_parent->DrawSprite(_elements.at(5).GetTexture(), _elements.at(5).GetTextureRect(), &topRect);
		_parent->DrawSprite(_elements.at(4).GetTexture(), _elements.at(4).GetTextureRect(), &buttonRect);
	} else if (_mouseOver)
	{
		_parent->DrawSprite(_elements.at(3).GetTexture(), _elements.at(3).GetTextureRect(), &buttonRect);
	} else 
	{
		_parent->DrawSprite(_elements.at(2).GetTexture(), _elements.at(2).GetTextureRect(), &buttonRect);
	}
}

bool AntGUIComboBox::Event(UINT uMsg, UINT p1, UINT p2)
{ 
	switch (uMsg)
	{
	case ANTGUI_EVENT_LBUTTONDOWN :
	case ANTGUI_EVENT_MOUSEMOVE:
		if (_opened)
		{
			POINT mouse = { short(LOWORD(p2)), short(HIWORD(p2)) };
			mouse.x -= _x;
			mouse.y -= _y;
			RECT optionRect = {0,0,Width(_elements.at(6).GetTextureRect()),Height(_elements.at(6).GetTextureRect())};
			bool found=false;
			for (unsigned int i=0; i<_options.size() && !found; i++)
			{
				OffsetRect(&optionRect,0,-Height(_elements.at(6).GetTextureRect()));
				if (mouse.x>=optionRect.left && mouse.x<=optionRect.right && mouse.y>=optionRect.top && mouse.y<=optionRect.bottom)
				{
					found=true;
					_hover=&_options.at(i);
				}
			}
			if (!found)
			{
				_hover=NULL;
				if (uMsg==ANTGUI_EVENT_LBUTTONDOWN) { _opened=false; UpdateBoundingBox();}
			} else
			{
				if (uMsg==ANTGUI_EVENT_LBUTTONDOWN)
				{
					_selected=_hover;
					_opened=false; UpdateBoundingBox();
					EventCall(NULL);
				}
			}
		} else 
		{
			if (uMsg==ANTGUI_EVENT_LBUTTONDOWN) { _opened=true; UpdateBoundingBox();}
		}
	break;
	}
	return false;
}

void AntGUIComboBox::UpdateBoundingBox()
{
	if (_opened)
	{
		_boundingBox.top=_y-((int)_options.size()*Height(_elements.at(6).GetTextureRect()))-Height(_elements.at(5).GetTextureRect());
	} else 
	{
		_boundingBox.top=_y;
	}
	_boundingBox.left=_x;
	_boundingBox.bottom=_y+_h;
	_boundingBox.right=_x+_w+Width(_elements.at(2).GetTextureRect());
}

void AntGUIComboBox::MouseExit()
{
	_mouseOver=false;
	_hover=NULL;
}
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUICheckBox::AntGUICheckBox(AntGUIDialog* parent)
{
	_type=ANTGUI_COMPONENT_CHECKBOX;

	AntGUIElement elem;
	RECT rct;

	elem.SetFont(0,AntFontColorARGB(255,0,0,0));						// Not Checked
	SetRect(&rct,179,19,198,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	elem.SetFont(0,AntFontColorARGB(255,0,0,0));						// Checked
	SetRect(&rct,198,19,217,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	_parent=parent;
	_checked=false;
	_mouseOver=false;
}

void AntGUICheckBox::Render(float time)
{
	if (_checked)
	{
		RECT text= {_x+Width(_elements.at(2).GetTextureRect()), _y, _x+_w-Width(_elements.at(2).GetTextureRect()), _y+_h};
		_parent->DrawSprite(_elements.at(2).GetTexture(), _elements.at(2).GetTextureRect(), &_boundingBox);
		_parent->DrawText(_elements.at(2).GetFont(), _text, _elements.at(2).GetFontColor(), &text);
	} else
	{
		RECT text= {_x+Width(_elements.at(1).GetTextureRect()), _y, _x+_w-Width(_elements.at(2).GetTextureRect()), _y+_h};
		_parent->DrawSprite(_elements.at(1).GetTexture(), _elements.at(1).GetTextureRect(), &_boundingBox);
		_parent->DrawText(_elements.at(1).GetFont(), _text,  _elements.at(1).GetFontColor(), &text);
	}
}

bool AntGUICheckBox::Event( UINT uMsg, UINT p1, UINT p2 )
{ 
	switch (uMsg)
	{
	case ANTGUI_EVENT_LBUTTONDOWN :
		_checked = (_checked) ? false : true;
		return true;
	break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUITextBox::AntGUITextBox(AntGUIDialog* parent)
{
	_type=ANTGUI_COMPONENT_TEXTBOX;

	AntGUIElement elem;
	RECT rct;

	elem.SetFont(0,AntFontColorARGB(255,255,255,255));						//	Background
	SetRect(&rct,64,19,127,38);
	elem.SetTexture(0,rct);
	_elements.push_back(elem);

	_parent=parent;
	_mouseOver=false;
	_pos=0;
}

void AntGUITextBox::Render(float time)
{
	_parent->DrawSprite(_elements.at(1).GetTexture(), _elements.at(1).GetTextureRect(), &_boundingBox);
	_parent->DrawText(_elements.at(1).GetFont(), _text,  _elements.at(1).GetFontColor(), &_boundingBox);
}

bool AntGUITextBox::Event( UINT uMsg, UINT p1, UINT p2 )
{ 
	switch (uMsg)
	{
	case ANTGUI_EVENT_KEYDOWN :
		switch(p1)
		{
			case ANTGUI_EVENT_KEY_LEFT:
			break;
			case ANTGUI_EVENT_KEY_BACK:
				_text[_pos]=0;
				SetCaret(_pos-1);
			break;
			default :
				_text[_pos]=(wchar_t)p1;
				SetCaret(_pos+1);
			break;
		}
	break;
	case ANTGUI_EVENT_KEYUP :
	break;
	case ANTGUI_EVENT_CHAR :
		switch(p1)
		{
		case ANTGUI_EVENT_KEY_LEFT:
		break;
		default :
			_text[_pos]=(wchar_t)p1;
			SetCaret(_pos+1);
		break;
		}
	break;
	default :
	break;
	}
	return false;
}

void AntGUITextBox::SetCaret(UINT pos)
{
	if (pos<MAX_PATH)
	{
		_pos=pos;
	}
}

void AntGUITextBox::MouseEnter()
{
	_mouseOver=true;
	//SetCursor(LoadCursor(NULL,IDC_IBEAM));
}

void AntGUITextBox::MouseExit()
{
	_mouseOver=false;
	//SetCursor(LoadCursor(NULL,IDC_ARROW));
}
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUITextArea::AntGUITextArea(AntGUIDialog* parent)
{
}

void AntGUITextArea::Render(float time)
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIListBox::AntGUIListBox(AntGUIDialog* parent)
{
}

void AntGUIListBox::Render(float time)
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUIProgressBar::AntGUIProgressBar(AntGUIDialog* parent)
{
}

void AntGUIProgressBar::Render(float time)
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntGUISlider::AntGUISlider(AntGUIDialog* parent)
{
}

bool AntGUISlider::Event( UINT uMsg, UINT p1, UINT p2 ) { return false; }

void AntGUISlider::Render(float time)
{
}
