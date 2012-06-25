/** @file
 * @brief �����I�u�W�F�N�g�E�C�x���g�̒�`
 *
 * @author ����  �O
 * @author sadmb modified at Oct. 26, 2011
 * $Date: 2004/10/21 11:25:09 $
 * $Revision: 1.1.1.1 $
 */
#include "win32/Event.h"

namespace win32 {
    Event::Event( LPCTSTR lpName, BOOL bManualReset /*= TRUE*/, BOOL bInitialState /*= FALSE*/,
                    LPSECURITY_ATTRIBUTES lpEventAttributes /*= 0*/ )
    {
        HANDLE handle = ::CreateEvent( lpEventAttributes, bManualReset, bInitialState, lpName );

        handle_ = Handle( handle, &::CloseHandle );

        // ���łɍ쐬�ς݂̃C�x���g���ǂ���
        exists_ = GetLastError() == ERROR_ALREADY_EXISTS;
    }

    Event::~Event()
    {
    }

    bool Event::Wait( DWORD dwMilliseconds /*= INFINITE*/ )
    {
        // �f�[�^�̍X�V��҂�
        DWORD ret = ::WaitForSingleObject( handle_.get(), dwMilliseconds );

        // �V�O�i����ԂɂȂ������A�^�C���A�E�g������
        return ret == WAIT_OBJECT_0;
    }
}
