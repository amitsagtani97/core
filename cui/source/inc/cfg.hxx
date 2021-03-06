/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */
#ifndef INCLUDED_CUI_SOURCE_INC_CFG_HXX
#define INCLUDED_CUI_SOURCE_INC_CFG_HXX

#include <vcl/fixed.hxx>
#include <vcl/group.hxx>
#include <vcl/layout.hxx>
#include <vcl/lstbox.hxx>
#include <vcl/menubtn.hxx>
#include <vcl/toolbox.hxx>
#include <vcl/weld.hxx>
#include <svtools/imgdef.hxx>
#include <svtools/miscopt.hxx>
#include <svtools/treelistbox.hxx>
#include <svtools/svmedit2.hxx>
#include <svtools/svmedit.hxx>

#include <comphelper/random.hxx>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/container/XIndexContainer.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/ui/XUIConfigurationListener.hpp>
#include <com/sun/star/ui/XUIConfigurationManager.hpp>
#include <com/sun/star/ui/XImageManager.hpp>
#include <com/sun/star/ui/ImageType.hpp>
#include <com/sun/star/ui/ItemType.hpp>
#include <com/sun/star/graphic/XGraphicProvider.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/ModuleManager.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/lang/XSingleComponentFactory.hpp>

#include <sfx2/tabdlg.hxx>
#include <memory>
#include <vector>

#include "cfgutil.hxx"
#include "CommandCategoryListBox.hxx"

static const char ITEM_DESCRIPTOR_COMMANDURL[]  = "CommandURL";
static const char ITEM_DESCRIPTOR_CONTAINER[]   = "ItemDescriptorContainer";
static const char ITEM_DESCRIPTOR_LABEL[]       = "Label";
static const char ITEM_DESCRIPTOR_TYPE[]        = "Type";
static const char ITEM_DESCRIPTOR_STYLE[]       = "Style";
static const char ITEM_DESCRIPTOR_ISVISIBLE[]   = "IsVisible";
static const char ITEM_DESCRIPTOR_RESOURCEURL[] = "ResourceURL";
static const char ITEM_DESCRIPTOR_UINAME[]      = "UIName";

static const char ITEM_MENUBAR_URL[] = "private:resource/menubar/menubar";
static const char ITEM_TOOLBAR_URL[] = "private:resource/toolbar/";

static const char CUSTOM_TOOLBAR_STR[] = "custom_toolbar_";

static const char aMenuSeparatorStr[] = " | ";

class SvxConfigEntry;
class SvxConfigPage;

typedef std::vector< SvxConfigEntry* > SvxEntries;

class SvxConfigDialog : public SfxTabDialog
{
private:
    css::uno::Reference< css::frame::XFrame > m_xFrame;
    sal_uInt16 m_nMenusPageId;
    sal_uInt16 m_nToolbarsPageId;
    sal_uInt16 m_nContextMenusPageId;
    sal_uInt16 m_nKeyboardPageId;
    sal_uInt16 m_nEventsPageId;

public:
    SvxConfigDialog( vcl::Window*, const SfxItemSet* );

    virtual void                PageCreated( sal_uInt16 nId, SfxTabPage &rPage ) override;
    void SetFrame(const css::uno::Reference< css::frame::XFrame >& xFrame);
};

class SaveInData
{
private:

    bool        bModified;

    bool        bDocConfig;
    bool        bReadOnly;

    css::uno::Reference
        < css::ui::XUIConfigurationManager > m_xCfgMgr;

    css::uno::Reference
        < css::ui::XUIConfigurationManager > m_xParentCfgMgr;

    css::uno::Reference
        < css::ui::XImageManager > m_xImgMgr;

    css::uno::Reference
        < css::ui::XImageManager > m_xParentImgMgr;

    static css::uno::Reference
        < css::ui::XImageManager >* xDefaultImgMgr;

protected:

    void ApplyMenu(
        css::uno::Reference< css::container::XIndexContainer > const & rMenuBar,
        css::uno::Reference< css::lang::XSingleComponentFactory >& rFactory,
        SvxConfigEntry *pMenuData );

    bool LoadSubMenus(
        const css::uno::Reference< css::container::XIndexAccess >& xMenuSettings,
        const OUString& rBaseTitle, SvxConfigEntry const * pParentData, bool bContextMenu );

public:

    SaveInData(
        const css::uno::Reference < css::ui::XUIConfigurationManager >& xCfgMgr,
        const css::uno::Reference < css::ui::XUIConfigurationManager >& xParentCfgMgr,
        const OUString& aModuleId,
        bool docConfig );

    virtual ~SaveInData() {}

    bool PersistChanges(
        const css::uno::Reference< css::uno::XInterface >& xManager );

    void SetModified( bool bValue = true ) { bModified = bValue; }
    bool IsModified( ) { return bModified; }

    bool IsReadOnly( ) { return bReadOnly; }
    bool IsDocConfig( ) { return bDocConfig; }

    const css::uno::Reference
        < css::ui::XUIConfigurationManager >&
            GetConfigManager() { return m_xCfgMgr; };

    const css::uno::Reference
        < css::ui::XUIConfigurationManager >&
            GetParentConfigManager() { return m_xParentCfgMgr; };

    const css::uno::Reference
        < css::ui::XImageManager >&
            GetImageManager() { return m_xImgMgr; };

    const css::uno::Reference
        < css::ui::XImageManager >&
            GetParentImageManager() { return m_xParentImgMgr; };

    css::uno::Reference
        < css::container::XNameAccess > m_xCommandToLabelMap;

    css::uno::Sequence
        < css::beans::PropertyValue > m_aSeparatorSeq;

    Image GetImage( const OUString& rCommandURL );

    virtual bool HasURL( const OUString& aURL ) = 0;
    virtual bool HasSettings() = 0;
    virtual SvxEntries* GetEntries() = 0;
    virtual void SetEntries( std::unique_ptr<SvxEntries> ) = 0;
    virtual void Reset() = 0;
    virtual bool Apply() = 0;
};

class MenuSaveInData : public SaveInData
{
private:

    OUString               m_aMenuResourceURL;
    OUString               m_aDescriptorContainer;

    css::uno::Reference
        < css::container::XIndexAccess > m_xMenuSettings;

    std::unique_ptr<SvxConfigEntry>      pRootEntry;


    static MenuSaveInData* pDefaultData;    ///< static holder of the default menu data

    static void SetDefaultData( MenuSaveInData* pData ) {pDefaultData = pData;}
    static MenuSaveInData* GetDefaultData() { return pDefaultData; }

    void        Apply(
        css::uno::Reference< css::container::XIndexContainer > const & rNewMenuBar,
        css::uno::Reference< css::lang::XSingleComponentFactory >& rFactory );

public:

    MenuSaveInData(
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const OUString& aModuleId,
        bool docConfig );

    virtual ~MenuSaveInData() override;

    /// methods inherited from SaveInData
    SvxEntries*         GetEntries() override;
    void                SetEntries( std::unique_ptr<SvxEntries> ) override;
    bool                HasURL( const OUString& ) override { return false; }
    bool                HasSettings() override { return m_xMenuSettings.is(); }
    void                Reset() override;
    bool                Apply() override;
};

class ContextMenuSaveInData : public SaveInData
{
private:
    std::unique_ptr< SvxConfigEntry > m_pRootEntry;
    css::uno::Reference< css::container::XNameAccess > m_xPersistentWindowState;
    OUString GetUIName( const OUString& rResourceURL );

public:
    ContextMenuSaveInData(
        const css::uno::Reference< css::ui::XUIConfigurationManager >& xCfgMgr,
        const css::uno::Reference< css::ui::XUIConfigurationManager >& xParentCfgMgr,
        const OUString& aModuleId, bool bIsDocConfig );
    virtual ~ContextMenuSaveInData() override;

    SvxEntries* GetEntries() override;
    void SetEntries( std::unique_ptr<SvxEntries> pNewEntries ) override;
    bool HasSettings() override;
    bool HasURL( const OUString& rURL ) override;
    void Reset() override;
    bool Apply() override;

    void ResetContextMenu( const SvxConfigEntry* pEntry );
};

class SvxConfigEntry
{
private:

    /// common properties
    sal_uInt16                  nId;
    OUString                    aLabel;
    OUString                    aCommand;

    bool                        bPopUp;
    bool                        bStrEdited;
    bool                        bIsUserDefined;
    bool                        bIsMain;
    bool                        bIsParentData;
    bool                        bIsModified;

    /// toolbar specific properties
    bool                        bIsVisible;
    sal_Int32                   nStyle;

    css::uno::Reference<
        css::graphic::XGraphic > xBackupGraphic;

    std::unique_ptr<SvxEntries>  mpEntries;

public:

    SvxConfigEntry( const OUString& rDisplayName,
                    const OUString& rCommandURL,
                    bool bPopup,
                    bool bParentData );

    SvxConfigEntry()
        :
            nId( 0 ),
            bPopUp( false ),
            bStrEdited( false ),
            bIsUserDefined( false ),
            bIsMain( false ),
            bIsParentData( false ),
            bIsModified( false ),
            bIsVisible( true ),
            nStyle( 0 ),
            mpEntries( nullptr )
    {}

    ~SvxConfigEntry();

    const OUString&      GetCommand() const { return aCommand; }

    const OUString&      GetName() const { return aLabel; }
    void                 SetName( const OUString& rStr ) { aLabel = rStr; bStrEdited = true; }
    bool                 HasChangedName() const { return bStrEdited; }

    bool    IsPopup() const { return bPopUp; }

    void    SetUserDefined( bool bOn = true ) { bIsUserDefined = bOn; }
    bool    IsUserDefined() const { return bIsUserDefined; }

    bool    IsBinding() const { return !bPopUp; }
    bool    IsSeparator() const { return nId == 0; }

    SvxEntries* GetEntries() const { return mpEntries.get(); }
    void    SetEntries( std::unique_ptr<SvxEntries> entries ) { mpEntries = std::move(entries); }

    void    SetMain() { bIsMain = true; }
    bool    IsMain() { return bIsMain; }

    void    SetParentData( bool bValue = true ) { bIsParentData = bValue; }
    bool    IsParentData() { return bIsParentData; }

    void    SetModified( bool bValue = true ) { bIsModified = bValue; }
    bool    IsModified() { return bIsModified; }

    bool    IsMovable();
    bool    IsDeletable();
    bool    IsRenamable();

    void    SetVisible( bool b ) { bIsVisible = b; }
    bool    IsVisible() const { return bIsVisible; }

    void    SetBackupGraphic( css::uno::Reference< css::graphic::XGraphic > const & graphic )
                { xBackupGraphic = graphic; }

    const css::uno::Reference< css::graphic::XGraphic >& GetBackupGraphic()
                { return xBackupGraphic; }

    sal_Int32   GetStyle() const { return nStyle; }
    void        SetStyle( sal_Int32 style ) { nStyle = style; }
};

class SvxMenuEntriesListBox : public SvTreeListBox
{
private:
    VclPtr<SvxConfigPage>      pPage;

protected:
    bool                m_bIsInternalDrag;

public:
    SvxMenuEntriesListBox(vcl::Window*, SvxConfigPage*);
    virtual ~SvxMenuEntriesListBox() override;
    virtual void dispose() override;

    virtual sal_Int8    AcceptDrop( const AcceptDropEvent& rEvt ) override;

    virtual bool        NotifyAcceptDrop( SvTreeListEntry* pEntry ) override;

    virtual TriState    NotifyMoving( SvTreeListEntry*, SvTreeListEntry*,
                                      SvTreeListEntry*&, sal_uLong& ) override;

    virtual TriState    NotifyCopying( SvTreeListEntry*, SvTreeListEntry*,
                                       SvTreeListEntry*&, sal_uLong&) override;

    virtual DragDropMode    NotifyStartDrag(
        TransferDataContainer&, SvTreeListEntry* ) override;

    virtual void        DragFinished( sal_Int8 ) override;

    void                KeyInput( const KeyEvent& rKeyEvent ) override;
};

class SvxConfigPage : public SfxTabPage
{
private:

    bool                                bInitialised;
    SaveInData*                         pCurrentSaveInData;

    DECL_LINK(  SelectSaveInLocation, ListBox&, void );
    DECL_LINK( SearchUpdateHdl, Edit&, void );

protected:

    // Left side of the dialog where command categories and the available
    // commands in them are displayed as a searchable list
    VclPtr<Edit>                               m_pSearchEdit;
    VclPtr<CommandCategoryListBox>             m_pCommandCategoryListBox;
    VclPtr<SfxConfigFunctionListBox>           m_pFunctions;

    VclPtr<FixedText>                          m_pDescriptionFieldLb;
    VclPtr<VclMultiLineEdit>                   m_pDescriptionField;

    // Right side of the dialog where the contents of the selected
    // menu or toolbar are displayed
    VclPtr<ListBox>                            m_pTopLevelListBox;
    // Used to add and remove toolbars/menus
    VclPtr<MenuButton>                         m_pGearBtn;
    VclPtr<VclContainer>                       m_pEntries;
    VclPtr<SvTreeListBox>                      m_pContentsListBox;

    VclPtr<PushButton>                         m_pMoveUpButton;
    VclPtr<PushButton>                         m_pMoveDownButton;

    VclPtr<ListBox>                            m_pSaveInListBox;

    VclPtr<MenuButton>                         m_pInsertBtn;
    VclPtr<MenuButton>                         m_pModifyBtn;
    // Used to reset the selected toolbar/menu/context menu
    VclPtr<PushButton>                         m_pResetBtn;

    // Middle buttons
    VclPtr<PushButton>                         m_pAddCommandButton;
    VclPtr<PushButton>                         m_pRemoveCommandButton;

    /// the ResourceURL to select when opening the dialog
    OUString                                   m_aURLToSelect;

    css::uno::Reference< css::frame::XFrame >  m_xFrame;

    SvxConfigPage( vcl::Window*, const SfxItemSet& );

    DECL_LINK( MoveHdl, Button *, void );
    DECL_LINK( SelectFunctionHdl, SvTreeListBox *, void );
    DECL_LINK( FunctionDoubleClickHdl, SvTreeListBox *, bool );

    virtual SaveInData* CreateSaveInData(
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const OUString& aModuleId,
        bool docConfig ) = 0;

    virtual void            Init() = 0;
    virtual void            UpdateButtonStates() = 0;
    virtual short           QueryReset() = 0;

    SvTreeListEntry*    InsertEntry(        SvxConfigEntry* pNewEntryData,
                                        SvTreeListEntry* pTarget = nullptr,
                                        bool bFront = false );

    void                AddSubMenusToUI(    const OUString& rBaseTitle,
                                        SvxConfigEntry const * pParentData );

    SvTreeListEntry*    InsertEntryIntoUI ( SvxConfigEntry* pNewEntryData,
                                        sal_uLong nPos = TREELIST_APPEND );

    SvxEntries*     FindParentForChild( SvxEntries* pParentEntries,
                                        SvxConfigEntry* pChildData );

    void            ReloadTopLevelListBox( SvxConfigEntry const * pSelection = nullptr );

public:

    virtual ~SvxConfigPage() override;
    virtual void dispose() override;

    static bool     CanConfig( const OUString& rModuleId );

    SaveInData*     GetSaveInData() { return pCurrentSaveInData; }

    SvTreeListEntry*    AddFunction( SvTreeListEntry* pTarget = nullptr,
                                 bool bFront = false,
                                 bool bAllowDuplicates = false );

    virtual void    MoveEntry( bool bMoveUp );

    bool            MoveEntryData(  SvTreeListEntry const * pSourceEntry,
                                    SvTreeListEntry const * pTargetEntry );

    bool            FillItemSet( SfxItemSet* ) override;
    void            Reset( const SfxItemSet* ) override;

    virtual void    DeleteSelectedContent() = 0;
    virtual void    DeleteSelectedTopLevel() = 0;

    SvxConfigEntry* GetTopLevelSelection()
    {
        return static_cast<SvxConfigEntry*>(m_pTopLevelListBox->GetEntryData(
            m_pTopLevelListBox->GetSelectedEntryPos() ));
    }

    /** identifies the module in the given frame. If the frame is <NULL/>, a default
        frame will be determined beforehand.

        If the given frame is <NULL/>, a default frame will be used: The method the active
        frame of the desktop, then the current frame. If both are <NULL/>,
        the SfxViewFrame::Current's XFrame is used. If this is <NULL/>, too, an empty string is returned.

        If the given frame is not <NULL/>, or an default frame could be successfully determined, then
        the ModuleManager is asked for the module ID of the component in the frame.
    */
    static OUString
        GetFrameWithDefaultAndIdentify( css::uno::Reference< css::frame::XFrame >& _inout_rxFrame );

    OUString    GetScriptURL() const;
    OUString    GetSelectedDisplayName();
};

class SvxMainMenuOrganizerDialog : public ModalDialog
{
    VclPtr<VclContainer>   m_pMenuBox;
    VclPtr<Edit>           m_pMenuNameEdit;
    VclPtr<SvTreeListBox>  m_pMenuListBox;
    VclPtr<PushButton>     m_pMoveUpButton;
    VclPtr<PushButton>     m_pMoveDownButton;

    std::unique_ptr<SvxEntries> mpEntries;
    SvTreeListEntry*    pNewMenuEntry;
    bool            bModified;

    void UpdateButtonStates();

    DECL_LINK( MoveHdl, Button *, void );
    DECL_LINK( ModifyHdl, Edit&, void );
    DECL_LINK( SelectHdl, SvTreeListBox*, void );

public:
    SvxMainMenuOrganizerDialog (
        vcl::Window*, SvxEntries*,
        SvxConfigEntry const *, bool bCreateMenu );
    virtual ~SvxMainMenuOrganizerDialog() override;
    virtual void dispose() override;

    std::unique_ptr<SvxEntries> ReleaseEntries() { return std::move(mpEntries);}
    SvxConfigEntry* GetSelectedEntry();
};

class ToolbarSaveInData : public SaveInData
{
private:

    std::unique_ptr<SvxConfigEntry>           pRootEntry;
    OUString                                  m_aDescriptorContainer;

    css::uno::Reference
        < css::container::XNameAccess > m_xPersistentWindowState;

    void        LoadToolbar(
        const css::uno::Reference< css::container::XIndexAccess >& xToolBarSettings,
        SvxConfigEntry const * pParentData );

    void        ApplyToolbar(
        css::uno::Reference< css::container::XIndexContainer > const & rNewToolbarBar,
        css::uno::Reference< css::lang::XSingleComponentFactory >& rFactory,
        SvxConfigEntry const *pToolbar );

public:

    ToolbarSaveInData(
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const css::uno::Reference< css::ui::XUIConfigurationManager >&,
        const OUString& aModuleId,
        bool docConfig );

    virtual ~ToolbarSaveInData() override;

    void            CreateToolbar( SvxConfigEntry* pToolbar );
    void            RestoreToolbar( SvxConfigEntry* pToolbar );
    void            RemoveToolbar( SvxConfigEntry* pToolbar );
    void            ApplyToolbar( SvxConfigEntry* pToolbar );

    OUString        GetSystemUIName( const OUString& rResourceURL );

    sal_Int32       GetSystemStyle( const OUString& rResourceURL );

    void            SetSystemStyle( const OUString& rResourceURL, sal_Int32 nStyle );

    void            SetSystemStyle(
                        const css::uno::Reference< css::frame::XFrame >& xFrame,
                        const OUString& rResourceURL, sal_Int32 nStyle );

    SvxEntries*     GetEntries() override;
    void            SetEntries( std::unique_ptr<SvxEntries> ) override;
    bool            HasSettings() override;
    bool            HasURL( const OUString& rURL ) override;
    void            Reset() override;
    bool            Apply() override;
};

class SvxNewToolbarDialog : public ModalDialog
{
private:
    VclPtr<Edit>           m_pEdtName;
    VclPtr<OKButton>       m_pBtnOK;

public:
    SvxNewToolbarDialog(vcl::Window* pWindow, const OUString& rName);
    virtual ~SvxNewToolbarDialog() override;
    virtual void dispose() override;

    VclPtr<ListBox>        m_pSaveInListBox;

    OUString GetName()
    {
        return m_pEdtName->GetText();
    }
};

class SvxIconSelectorDialog : public ModalDialog
{
private:
    VclPtr<ToolBox>        pTbSymbol;
    VclPtr<FixedText>      pFtNote;
    VclPtr<PushButton>     pBtnImport;
    VclPtr<PushButton>     pBtnDelete;
    Size            aTbSize;
    sal_uInt16      m_nNextId;

    sal_Int32       m_nExpectedSize;

    css::uno::Reference<
        css::ui::XImageManager > m_xImageManager;

    css::uno::Reference<
        css::ui::XImageManager > m_xParentImageManager;

    css::uno::Reference<
        css::ui::XImageManager > m_xImportedImageManager;

    css::uno::Reference<
        css::graphic::XGraphicProvider > m_xGraphProvider;

    bool ReplaceGraphicItem( const OUString& aURL );

    bool ImportGraphic( const OUString& aURL );

    void ImportGraphics( const css::uno::Sequence< OUString >& aURLs );

public:

    SvxIconSelectorDialog(
        vcl::Window *pWindow,
        const css::uno::Reference< css::ui::XImageManager >& rXImageManager,
        const css::uno::Reference< css::ui::XImageManager >& rXParentImageManager
            );

    virtual ~SvxIconSelectorDialog() override;
    virtual void dispose() override;

    css::uno::Reference< css::graphic::XGraphic >
        GetSelectedIcon();

    DECL_LINK( SelectHdl, ToolBox *, void );
    DECL_LINK( ImportHdl, Button *, void );
    DECL_LINK( DeleteHdl, Button *, void );
};

//added for issue83555
class SvxIconChangeDialog : public weld::MessageDialogController
{
private:
    std::unique_ptr<weld::TextView> m_xLineEditDescription;
public:
    SvxIconChangeDialog(weld::Window *pWindow, const OUString& rMessage);
};

#endif // INCLUDED_CUI_SOURCE_INC_CFG_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
