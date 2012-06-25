/** @file
* @brief �����I�u�W�F�N�g�E�C�x���g�̐錾
*
* @author ����  �O
* $Date: 2004/10/21 11:25:09 $
* $Revision: 1.1.1.1 $
*/
#if !defined( EVENT_H_INCLUDE )
#define EVENT_H_INCLUDE

#include <memory>

namespace std {
	using namespace std::tr1;
}

// MFC�g�p���p�ɂ�windows.h�̓C���N���[�h���Ȃ�
#if !defined( __AFXWIN_H__ )
#include <windows.h>
#endif  // !defined( __AFXWIN_H__ )

/// Win32���b�p�[�N���X�Q
namespace win32
{
    /// �C�x���g
    class Event
    {
    public:

        typedef std::shared_ptr< void > Handle;

        Event( LPCTSTR lpName, BOOL bManualReset = TRUE, BOOL bInitialState = FALSE,
               LPSECURITY_ATTRIBUTES lpEventAttributes = 0 );
        ~Event();

        bool Wait( DWORD dwMilliseconds = INFINITE );

        HANDLE get() { return handle_.get(); }

        // ���̃C�x���g�����ɊJ����Ă������̂��ǂ���
        bool IsExists() const { return exists_; }

    private:

        Handle handle_; ///< �C�x���g�n���h��
        bool   exists_; ///< ���̃C�x���g�����ɊJ����Ă������̂��ǂ���
    };

} // namespace win32

#endif // !defined( EVENT_H_INCLUDE )
// EOF
