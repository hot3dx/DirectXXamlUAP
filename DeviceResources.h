#pragma once

namespace DX
{
	// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};

	static const UINT c_frameCount = 3;		// Use triple buffering.

	// Controls all the DirectX device resources.
	class DeviceResources
	{
	public:
		DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
		void SetSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
		void SetWindow(Windows::UI::Core::CoreWindow^ window);
		void SetLogicalSize(Windows::Foundation::Size logicalSize);
		void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation);
		void SetDpi(float dpi);
		void ValidateDevice();
		void Present();
		void WaitForGpu();
		void SetCompositionScale(float compositionScaleX, float compositionScaleY);
		void Trim();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);

		// The size of the render target, in pixels.
		Windows::Foundation::Size	GetOutputSize() const { return m_outputSize; }

		// The size of the render target, in dips.
		Windows::Foundation::Size	GetLogicalSize() const { return m_logicalSize; }

		float						GetDpi() const { return m_effectiveDpi; }
		bool						IsDeviceRemoved() const { return m_deviceRemoved; }

		// D3D Accessors.
		ID3D12Device* GetD3DDevice() const { return m_d3dDevice.Get(); }
		IDXGISwapChain3* GetSwapChain() const { return m_swapChain.Get(); }
		ID3D12Resource* GetRenderTarget() const { return m_renderTargets[m_currentFrame].Get(); }
		ID3D12Resource* GetDepthStencil() const { return m_depthStencil.Get(); }
		ID3D12CommandQueue* GetCommandQueue() const { return m_commandQueue.Get(); }
		ID3D12CommandAllocator* GetCommandAllocator() const { return m_commandAllocators[m_currentFrame].Get(); }
		DXGI_FORMAT					GetBackBufferFormat() const { return m_backBufferFormat; }
		DXGI_FORMAT					GetDepthBufferFormat() const { return m_depthBufferFormat; }
		D3D12_VIEWPORT				GetScreenViewport() const { return m_screenViewport; }
		DirectX::XMFLOAT4X4			GetOrientationTransform3D() const { return m_orientationTransform3D; }
		UINT						GetCurrentFrameIndex() const { return m_currentFrame; }
		Windows::UI::Xaml::Controls::SwapChainPanel^ GetSwapChainPanel() const { return m_swapChainPanel; }
		void                        GetCreateDeviceResources() { CreateDeviceResources(); }
		ID2D1DeviceContext*         GetD3DDeviceContext() {	return m_d2dContext.Get();}
		ID3D12DescriptorHeap* GetRtvHeap() { return m_rtvHeap.Get(); }

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		}

		// D2D Accessors.
		ID2D1Factory3* GetD2DFactory() const { return m_d2dFactory.Get(); }
		ID2D1Device2* GetD2DDevice() const { return m_d2dDevice.Get(); }
		ID2D1DeviceContext2* GetD2DDeviceContext() const { return m_d2dContext2.Get(); }
		ID2D1Bitmap1* GetD2DTargetBitmap() const { return m_d2dTargetBitmap.Get(); }
		IDWriteFactory3* GetDWriteFactory() const { return m_dwriteFactory.Get(); }
		IWICImagingFactory2* GetWicImagingFactory() const { return m_wicFactory.Get(); }
		D2D1::Matrix3x2F			GetOrientationTransform2D() const { return m_orientationTransform2D; }


	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();
		void MoveToNextFrame();
		DXGI_MODE_ROTATION ComputeDisplayRotation();
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);


		UINT											m_currentFrame;


		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D12Device>			m_d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4>			m_dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>			m_swapChain;

		Microsoft::WRL::ComPtr<ID3D12Resource>			m_renderTargets[c_frameCount];

		Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>		m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	m_commandAllocators[c_frameCount];
		DXGI_FORMAT										m_backBufferFormat;
		DXGI_FORMAT										m_depthBufferFormat;
		D3D12_VIEWPORT									m_screenViewport;
		UINT											m_rtvDescriptorSize;
		bool											m_deviceRemoved;

		// CPU/GPU Synchronization.
		Microsoft::WRL::ComPtr<ID3D12Fence>				m_fence;
		UINT64											m_fenceValues[c_frameCount];
		HANDLE											m_fenceEvent;

		// Cached reference to the XAML panel.
		Windows::UI::Xaml::Controls::SwapChainPanel^ m_swapChainPanel;

		// Cached reference to the Window.
		Platform::Agile<Windows::UI::Core::CoreWindow>	m_window;

		Microsoft::WRL::ComPtr<ID2D1Factory3>           m_d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1Device1>            m_d2dDevice1;
		Microsoft::WRL::ComPtr<ID2D1Device2>		    m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>            m_d2dTargetBitmap;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>            m_d2dTargetBitmapRight;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>      m_d2dContext;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext2>  	m_d2dContext2;

		// DirectWrite drawing components.
		Microsoft::WRL::ComPtr<IDWriteFactory2>         m_dwriteFactory2;
		Microsoft::WRL::ComPtr<IDWriteFactory3>         m_dwriteFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory2>     m_wicFactory;

		// Cached device properties.
		Windows::Foundation::Size						m_d3dRenderTargetSize;
		Windows::Foundation::Size						m_outputSize;
		Windows::Foundation::Size						m_logicalSize;
		Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
		Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;
		float											m_dpi;
		float											m_compositionScaleX;
		float											m_compositionScaleY;
		float											m_effectiveCompositionScaleX;
		float											m_effectiveCompositionScaleY;
		bool                                            m_stereoEnabled;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_textBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>       m_textFormat;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>       m_smallTextFormat;


		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float											m_effectiveDpi;

		// Transforms used for display orientation.
		D2D1::Matrix3x2F                                m_orientationTransform2D;
		DirectX::XMFLOAT4X4								m_orientationTransform3D;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* m_deviceNotify;

	};
}

