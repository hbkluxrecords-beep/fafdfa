#pragma once
#include "../../utils/ud_layer.h"
#define STB_IMAGE_IMPLEMENTATION
#define IMGUI_DEFINE_MATH_OPERATORS
#include "custom_overlay.h"
#include "menu.h"
#include <d3d11.h>
#include <dwmapi.h>
#include "../../world/actor_loops/visual.h"
#include "../../utils/protection/ex.h"
#include "bytes.h"
#include "stb_image.h"

static HWND Window = NULL;
ID3D11Device* g_pd3dDevice;
ID3D11DeviceContext* g_pd3dDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_mainRenderTargetView;
HWND hwnd = NULL;
MSG Message = { NULL };
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

__forceinline void CreateRenderTarget ( )
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer ( 0, IID_PPV_ARGS ( &pBackBuffer ) );
	g_pd3dDevice->CreateRenderTargetView ( pBackBuffer, NULL, &g_mainRenderTargetView );
	pBackBuffer->Release ( );
}
__forceinline void CleanupRenderTarget ( )
{
	if ( g_mainRenderTargetView ) { g_mainRenderTargetView->Release ( ); g_mainRenderTargetView = NULL; }
}
__forceinline void CleanupDeviceD3D ( )
{
	CleanupRenderTarget ( );
	if ( g_pSwapChain ) { g_pSwapChain->Release ( ); g_pSwapChain = NULL; }
	if ( g_pd3dDeviceContext ) { g_pd3dDeviceContext->Release ( ); g_pd3dDeviceContext = NULL; }
	if ( g_pd3dDevice ) { g_pd3dDevice->Release ( ); g_pd3dDevice = NULL; }
}

void ovrl ( HWND win, bool ins ) {
	if (ins) {
		SetWindowLong (win, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST );
	}
	else {
		SetWindowLong (win, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST );
	}
	SetWindowPos (win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );
}


__forceinline auto MainRender ( ) -> void
{
	ImGui_ImplDX11_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );

	if ( GetAsyncKeyState ( VK_INSERT ) & 1 ) {
		settings::menu = !settings::menu;
	}

	if ( settings::menu ) {
		settings::menu = true;
		ovrl( Window, true );
	}
	else
	{ 
		settings::menu = false;
		ovrl( Window, false );
	}

	if ( aim::show_crosshair ) {
		ImVec2 center ( GetSystemMetrics ( SM_CXSCREEN ) / 2, GetSystemMetrics ( SM_CYSCREEN ) / 2 );
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x -  1.1f, center.y - 9.0f ), 
			ImVec2 ( center.x - 1.1f, center.y + 9.0f ),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x + 1.1f, center.y - 9.0f ),
			ImVec2 ( center.x + 1.1f, center.y + 9.0f ),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x, center.y - 9.0f - 1.1f),
			ImVec2 ( center.x, center.y + 9.0f + 1.1f),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x, center.y - 9.0f + 1.1f),
			ImVec2 ( center.x, center.y + 9.0f - 1.1f),
			ImColor(0, 0, 0, 255), 3.0f
		);

		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x - 9.0f - 1.1f, center.y ),
			ImVec2 ( center.x + 9.0f + 1.1f, center.y ),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x - 9.0f + 1.1f, center.y ),
			ImVec2 ( center.x + 9.0f - 1.1f, center.y ),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x - 9.0f, center.y - 1.1f),
			ImVec2 ( center.x + 9.0f, center.y - 1.1f),
			ImColor(0, 0, 0, 255), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x - 9.0f, center.y + 1.1f),
			ImVec2 ( center.x + 9.0f, center.y + 1.1f),
			ImColor(0, 0, 0, 255), 3.0f
		);

		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x, center.y - 9.0f ),
			ImVec2 ( center.x, center.y + 9.0f ),
			ImColor(colors::crosshair[0], colors::crosshair[1], colors::crosshair[2], colors::crosshair[3]), 3.0f
		);
		ImGui::GetForegroundDrawList ( )->AddLine (
			ImVec2 ( center.x - 9.0f, center.y ),
			ImVec2 ( center.x + 9.0f, center.y ),
			ImColor(colors::crosshair[0], colors::crosshair[1], colors::crosshair[2], colors::crosshair[3]), 3.0f
		);
	}



	if ( aim::show_fov ) {
		ImGui::GetForegroundDrawList ( )->AddCircle ( ImVec2 ( GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), aim::fov, ImColor ( colors::fov [ 0 ], colors::fov [ 1 ], colors::fov [ 2 ], colors::fov [ 3 ] ), 128, 1.f );
	}

	actor_loop ( );
	if ( settings::menu ) {
		menu::menu ( );
	}

	switch (aim::aim_bindss) {
	case 0: aim::current_bind = VK_LBUTTON; break;  
	case 1: aim::current_bind = VK_RBUTTON; break;  
	case 2: aim::current_bind = VK_XBUTTON1; break;   
	case 3: aim::current_bind = VK_XBUTTON2; break;  
	case 4: aim::current_bind = VK_SHIFT; break;    
	case 5: aim::current_bind = VK_CONTROL; break;    
	case 6: aim::current_bind = VK_MENU; break;       
	case 7: aim::current_bind = VK_CAPITAL; break;    
	default: aim::current_bind = VK_LBUTTON;
	}


	ImGui::EndFrame ( );
	ImGui::Render ( );
	ImVec4 clear_color = ImVec4 ( 0., 0., 0., 0. );
	g_pd3dDeviceContext->OMSetRenderTargets ( 1, &g_mainRenderTargetView, NULL );
	g_pd3dDeviceContext->ClearRenderTargetView ( g_mainRenderTargetView, ( float* ) &clear_color );
	ImGui_ImplDX11_RenderDrawData ( ImGui::GetDrawData ( ) );
	g_pSwapChain->Present ( 0, 0 );
}


namespace imguisetup {

	bool LoadTextureFromMemory ( const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
	{
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load_from_memory ( ( const unsigned char* ) data, ( int ) data_size, &image_width, &image_height, NULL, 4 );
		if ( image_data == NULL )
			return false;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory ( &desc, sizeof ( desc ) );
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		g_pd3dDevice->CreateTexture2D ( &desc, &subResource, &pTexture );

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory ( &srvDesc, sizeof ( srvDesc ) );
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		g_pd3dDevice->CreateShaderResourceView ( pTexture, &srvDesc, out_srv );
		pTexture->Release ( );

		*out_width = image_width;
		*out_height = image_height;
		stbi_image_free ( image_data );

		return true;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	__forceinline auto Overlay ( ) -> void
	{

		WNDCLASSEX wc = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WndProc,
			0L, 0L,
			GetModuleHandle(NULL),
			NULL, NULL, NULL, NULL,
			ud::g_wnd_class.c_str(),
			NULL
		};

		RegisterClassEx(&wc);

		Window = CreateWindowExA(
			WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
			ud::g_wnd_class.c_str(),
			ud::g_wnd_title.c_str(),
			WS_POPUP,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL,
			wc.hInstance,
			NULL
		);

		SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);
		MARGINS Margin = { -1 };
		DwmExtendFrameIntoClientArea(Window, &Margin);

		ShowWindow(Window, SW_SHOW);
		UpdateWindow(Window);
		ovrl( Window, settings::menu );
		
	}

	__forceinline auto SetupImgui ( ) -> HRESULT
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		ZeroMemory ( &SwapChainDesc, sizeof ( SwapChainDesc ) );
		SwapChainDesc.Windowed = TRUE;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		SwapChainDesc.BufferDesc.Height = 0;
		SwapChainDesc.BufferDesc.Width = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = Window;
		SwapChainDesc.SampleDesc.Count = 8; // 8
		SwapChainDesc.SampleDesc.Quality = 0;
		UINT createDeviceFlags = 0;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray [ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if ( D3D11CreateDeviceAndSwapChain ( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &SwapChainDesc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext ) != S_OK )
			return false;

		IMGUI_CHECKVERSION ( );
		ImGui::CreateContext ( );
		ImGuiIO& io = ImGui::GetIO ( ); ( void ) io;

		ImGuiStyle& style = ImGui::GetStyle ( );

		style.WindowPadding = ImVec2 ( 0, 0 );
		style.ItemSpacing = ImVec2 ( 20, 20 );
		style.WindowBorderSize = 0;
		style.ScrollbarSize = 6.f;
		style.ChildBorderSize = 0.f;
		ImFontConfig cfg;
		io.ConfigDebugHighlightIdConflicts = false;
		font::lexend_general_bold = io.Fonts->AddFontFromMemoryTTF(lexend_bold, sizeof(lexend_bold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::lexend_bold = io.Fonts->AddFontFromMemoryTTF(lexend_regular, sizeof(lexend_regular), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::lexend_regular = io.Fonts->AddFontFromMemoryTTF(lexend_regular, sizeof(lexend_regular), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::icomoon = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

		font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF(icomoon_widget, sizeof(icomoon_widget), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::icomoon_widget2 = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

		ID3D11Texture2D* pBackBuffer;
		g_pSwapChain->GetBuffer ( 0, IID_PPV_ARGS ( &pBackBuffer ) );
		g_pd3dDevice->CreateRenderTargetView ( pBackBuffer, NULL, &g_mainRenderTargetView );

		ImGui_ImplWin32_Init ( Window );
		ImGui_ImplDX11_Init ( g_pd3dDevice, g_pd3dDeviceContext );

		pBackBuffer->Release ( );
	}

	__forceinline auto GuiLoop ( ) -> void
	{
		static RECT old_rc;
		ZeroMemory ( &Message, sizeof ( MSG ) );

		while ( Message.message != WM_QUIT )
		{
			if ( PeekMessageA ( &Message, Window, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage ( &Message );
				DispatchMessageA ( &Message );
			}

			ImGuiIO& io = ImGui::GetIO ( );
			POINT p;
			POINT xy;

			GetCursorPos ( &p );
			long styleProc = GetWindowLongA ( Window, GWL_EXSTYLE );
			io.MousePos.x = p.x;
			io.MousePos.y = p.y;
			io.DeltaTime = 1.0f / 60.0f;
			io.ImeWindowHandle = Window;

			if ( GetAsyncKeyState ( VK_LBUTTON ) ) {
				io.MouseDown [ 0 ] = true;
				io.MouseClicked [ 0 ] = true;
				io.MouseClickedPos [ 0 ].x = io.MousePos.x;
				io.MouseClickedPos [ 0 ].x = io.MousePos.y;
			}
			else
				io.MouseDown [ 0 ] = false;


			MainRender ( );
		}
		ImGui_ImplDX11_Shutdown ( );
		ImGui_ImplWin32_Shutdown ( );
		ImGui::DestroyContext ( );

		CleanupDeviceD3D ( );
	}
}