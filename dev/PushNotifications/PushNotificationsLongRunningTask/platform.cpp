﻿#include "pch.h"

#include "platform.h"
#include "platformfactory.h"

void NotificationsLongRunningPlatformImpl::Initialize()
{
    auto lock = m_lock.lock_exclusive();
    THROW_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);

    if (m_initialized)
    {
        return;
    }

    // Schedule event signaling after 5 seconds. This is in case we don't have any apps to track in the LRP.
    // If we realize that we need to persist the LRP, timer should be canceled.
    m_shutdownTimerManager = std::make_unique<PlatformLifetimeTimerManager>();
    m_shutdownTimerManager->Setup();

    /* TODO: Verify registry and UDK list and make sure we have apps to be tracked */

    std::vector<std::wstring> appList;

    /* TODO: Load platform components */

    m_notificationListenerManager.Initialize(this, appList);

    m_initialized = true;
}

void NotificationsLongRunningPlatformImpl::Shutdown() noexcept
{
    auto lock = m_lock.lock_exclusive();
    if (m_shutdown)
    {
        return;
    }

    /* TODO: Shut down your components */

    m_shutdown = true;
}

void NotificationsLongRunningPlatformImpl::WaitForWinMainEvent()
{
    THROW_HR_IF_NULL(E_UNEXPECTED, m_shutdownTimerManager.get());
    m_shutdownTimerManager->Wait();
}

// Example of one function. We will add more as we need them.
STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::RegisterFullTrustApplication(_In_ PCWSTR /*processName*/, _In_ GUID /*remoteId*/, _Out_ GUID* /*appId*/) noexcept
{
    auto lock = m_lock.lock_shared();
    RETURN_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);
    
    return E_NOTIMPL;
}


STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::RegisterActivator(_In_ PCWSTR processName) noexcept
{
    auto lock = m_lock.lock_shared();
    RETURN_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);

    // TODO: Look if the process name exists in the Storage

    m_notificationListenerManager.AddListener(processName);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::UnregisterActivator(_In_ PCWSTR processName) noexcept
{
    auto lock = m_lock.lock_shared();
    RETURN_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);

    // TODO: Look if the process name exists in the Storage

    m_notificationListenerManager.RemoveListener(processName);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::RegisterForegroundActivator(_In_ IWpnForegroundSink* sink, _In_ PCWSTR processName)
{
    RETURN_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);
    auto lock = m_lock.lock_exclusive();

    m_foregroundSinkManager.Add(processName, sink);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::UnregisterForegroundActivator(_In_ PCWSTR processName)
{
    RETURN_HR_IF(WPN_E_PLATFORM_UNAVAILABLE, m_shutdown);
    auto lock = m_lock.lock_exclusive();

    m_foregroundSinkManager.Remove(processName);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) NotificationsLongRunningPlatformImpl::SendBackgroundNotification(_In_ PCWSTR processName, _In_ ULONG payloadSize, _In_ byte* payload)
{
    return S_OK;
}
