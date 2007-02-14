/***************************************************************************
                          kst.h  -  description
                             -------------------
    begin                : Tue Aug 22 13:46:13 CST 2000
    copyright            : (C) 2000 by Barth Netterfield
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KST_H
#define KST_H

#include <config.h>

// include files for Qt
#include <qguardedptr.h>
#include <qtimer.h>

// include files for KDE
#include <kmdimainfrm.h>

// application specific includes
#include "kst_export.h"
#include "kstpainter.h"
#include "ksttoplevelview.h"

enum LegendType { LegendOn, LegendOff, LegendAuto };
enum DataType { DataOnly, DataPSD, PSDOnly };
enum LineType { LineOnly, PointOnly, LinePoint };
enum PlotType { OnePlot, MultiplePlots, CycleThrough, ExistingPlot, ExistingCycle};

// forward declaration of the Kst classes
class GraphicEditorI;
class Kst2DPlot;
typedef KstObjectMap<KstSharedPtr<Kst2DPlot> > Kst2DPlotMap;
class KstChangeFileDialogI;
class KstChangeNptsDialogI;
class KstChooseColorDialogI;
class KstCurveDifferentiateI;
class KstDataManagerI;
class KstDataNotifier;
class KstDebugDialogI;
class KstDebugNotifier;
class KstDoc;
class KstEventMonitorEntry;
class KstEventMonitorI;
class KstGraphFileDialogI;
class KstIfaceImpl;
class KstMonochromeDialogI;
class KstPlotDialogI;
class KstQuickStartDialogI;
class KstTopLevelView;
class KstViewFitsDialogI;
class KstViewManagerI;
class KstViewMatricesDialogI;
class KstViewScalarsDialogI;
class KstViewStringsDialogI;
class KstViewVectorsDialogI;
class KstViewWindow;
class StatusLabel;
class UpdateThread;
class VectorSaveDialog;

class QLabel;
class QProgressBar;
class KProgress;


class KstOpen {
  public:
    QString filename, file;
    int n, f, s;
    bool ave;
};

/** The base class for Kst application windows. It sets up the main
  * window frame, reads the config file.
  * KstApp reimplements the methods that KMdiMainFrm provides.
  * @see KMdiMainFrm @see KApplication @see KConfig
  *
  * @author original Source Framework automatically generated by
  * KDevelop, (c) The KDevelop Team.  Modified by C. Barth Netterfield
  */
class KST_EXPORT KstApp : public KMdiMainFrm {
  Q_OBJECT
  public:
    static void initialize(); // for main to call

    KstApp(QWidget* parent=0, const char* name=0);
    ~KstApp();

    virtual QSize sizeHint() const;

    static KstApp* inst();
    static void doubleCheckCleanup();

    void checkFontPresent(const QString& font);
    const QString& defaultFont() const;

    /** add a file to the recent file list */
    void addRecentFile(const KURL &file);
    void selectRecentFile(const KURL &file);

    /** opens a file specified by commandline option */
    bool openDocumentFile(const QString& _cmdl = QString::null,
        const QString& o_file = "|", int o_n = -2, int o_f = -2,
        int o_s = -1, bool o_ave = false, bool delayed = false);

    /** returns a pointer to the current document connected to the
     * KMainWindow instance and is used by the View class to access
     * the document object's methods */
    KstDoc *document() const;

    /** returns a pointer to the monochrome settings dialog */
    KstMonochromeDialogI *monochromeDialog() const;

    /** pause the updating of data */
    void setPaused(bool paused);
    void togglePaused();

    enum KstZoomType { XYZOOM, YZOOM, XZOOM, TEXT, GRAPHICS, LAYOUT };
    enum KstGraphicType { GFX_LINE, GFX_ARROW, GFX_POLYGON, GFX_POLYLINE, GFX_ELLIPSE, 
                           GFX_RECTANGLE, GFX_ROUNDED_RECTANGLE, GFX_CURVE, GFX_LABEL };
    /** Get XY zoom radio button state */
    KstZoomType getZoomRadio();
    KstGraphicType getGraphicType();
    bool saveData() const { return _saveData->isChecked(); }

    KstTopLevelView::ViewMode currentViewMode();
    QString currentCreateType();
    
    KstTopLevelViewPtr activeView();
    Kst2DPlotMap* plotHolderWhileOpeningDocument();

    QString windowName(bool prompt, const QString& nameOriginal, bool rename = false, QWidget *parent = 0L);
    QString newWindow(const QString& name);
    QString newWindow(bool prompt = true, QWidget *parent=0L);
    void renameWindow(KstViewWindow *);
    void tiedZoomPrev(KstViewWidget* view, const QString& plotName);
    void tiedZoomMode(int zoom, bool flag, double center, int mode, int modeExtra, KstViewWidget* view, const QString& plotName);
    void tiedZoom(bool x, double xmin, double xmax, bool y, double ymin, double ymax, KstViewWidget* view, const QString& plotName);
    
    bool dataMode() const;

    void paintAll(KstPainter::PaintType = KstPainter::P_PAINT);

    void EventELOGSubmitEntry(const QString& message);

    //return the list of recent files
    const QStringList recentFiles() const;

    KConfig *dataSourceConfig() const { return _dataSourceConfig; }

    KstGraphFileDialogI *graphFileDlg() const { return graphFileDialog; }

    bool paused() const;

    void moveTabLeft(KstViewWindow*);
    void moveTabRight(KstViewWindow*);

  protected:
    void customEvent(QCustomEvent *e);


    /** save options to the configuration file
     *  Geometry, Toolbar status, Statusbar status */
    void saveOptions();

    /** read options from configuration file
     *  Geometry, Toolbar status, Statusbar status */
    void readOptions();

    /** setup kde2 actions and build the GUI */
    void initActions();

    /** sets up the statusbar for the main window */
    void initStatusBar();

    /** initializes the document object */
    void initDocument();

    /** queryClose is called by KMainWindow on each closeEvent of a
     * window. This calles saveModified() on the document object to ask
     * if the document should be saved if Modified; on cancel the
     * closeEvent is rejected.
     * @see saveModified()
     * @see KMainWindow#queryClose
     * @see KMainWindow#closeEvent */
    bool queryClose();

    /** Calls queryClose */
    bool queryExit();

    /** saves the window properties for each open window during session
     * end to the session config file, including saving the currently
     * opened file by a temporary filename provided by KApplication.
     * @see KMainWindow#saveProperties */
    void saveProperties(KConfig *cfg);

    /** reads the session config file and restores the application's
     * state including the last opened files and documents by reading
     * the temporary files saved by saveProperties()
     * @see KMainWindow#readProperties */
    void readProperties(KConfig *cfg);

  private slots:
    // Hack to update KStdActions
    void updateActions();
    void loadExtensions();

    // for the kaction
    void toggleDataMode();
    void toggleMouseMode();

    void slotSettingsChanged();
    void addNewWindowMenu();

    void fixKMdi();
    void showContextMenu(QWidget *w, const QPoint& pos);
    void showContextMenu(const QPoint& pos);
    void delayedDocInit();
    void selectDataPlugin();

  public slots:
    void fromEnd();

    void updatePausedState(bool);

    void tieAll();

    void forceUpdate();

    void slotConfigureKeys();

    /** open a new application window */
    void slotFileNewWindow(QWidget *parent = 0L);

    /** rename aa existing application window */
    void slotFileRenameWindow();

    /** clears the document in the view and creates a new document */
    void slotFileNew();

    /** open a file and load it into the document*/
    void slotFileOpen();

    /** opens a file from the recent files menu */
    bool slotFileOpenRecent(const KURL &);

    /** save a document */
    void slotFileSave();

    /** save a document by a new filename*/
    bool slotFileSaveAs();

    /** asks for saving if the file is modified, then closes the actual
      file and window*/
    void slotFileClose();

    /** closes all windows and exits program: not used */
    void slotFileQuit();

    /** print the actual file */
    void slotFilePrint();

    /** print without querying */
    void immediatePrintToFile(const QString& filename, bool revert = true);
    void immediatePrintWindowToFile(KMdiChildView *window, const QString& filename);
    void immediatePrintActiveWindowToFile(const QString& filename);

    /** export to png without querying */
    void immediatePrintToPng(const QString& filename, const QString& format = "PNG", int width = 640, int height = 480, bool all = false, int display = 0);
    void immediatePrintWindowToPng(KMdiChildView *window, const QString& filename, const QString& format = "PNG", int width = 640, int height = 480, int display = 0);
    void immediatePrintActiveWindowToPng(const QString& filename, const QString& format = "PNG", int width = 640, int height = 480, int display = 0);

    /** export to eps without querying */    
    void immediatePrintToEps(const QString& filename, int width = 640, int height = 480, bool all=false, int display = 0);
    void immediatePrintWindowToEps(KMdiChildView *win, const QString& filename, int width, int height, int display);    
    void immediatePrintActiveWindowToEps(const QString& filename, int width, int height, int display);
    
    //void slotCut() {}
    void slotCopy();
    void slotPaste();

    /** toggles the statusbar */
    void slotViewStatusBar();

    /** changes the statusbar contents */
    void updateStatusBarText();
    void slotUpdateStatusMsg(const QString &msg);
    void slotUpdateDataMsg(const QString &msg);
    void slotUpdateMemoryMsg(const QString &msg);
    void slotUpdateProgress(int total, int step, const QString &msg);

    /** creates a new plot in the current window */
    void newPlot();

    /** just calls dataManager->show_I(0) */
    void showDataManager();
    void showViewManager();
    
    /** just calls changeFileDialog->showChangeFileDialog(0) */
    void showChangeFileDialog();
    
    /** just calls monochromeDialog->showMonochromeDialog(0) */
    void showMonochromeDialog();
    
    /** just calls chooseColorDialog->showChooseColorDialog(0) */
    void showChooseColorDialog();

    /** just calls chooseColorDialog->showChooseColorDialog(0) */
    void showDifferentiateCurvesDialog();
    
    /** just calls viewScalarsDialog->showViewScalarsDialog(0) */
    void showViewScalarsDialog();

    /** just calls viewStringsDialog->showViewStringsDialog(0) */
    void showViewStringsDialog();

    /** just calls viewVectorsDialog->showViewVectorsDialog(0) */
    void showViewVectorsDialog();

    /** just calls viewMatricesDialog->showViewMatricesDialog(0) */
    void showViewMatricesDialog();
    
    /** just calls viewFitsDialog->showViewFitsDialog(0) */
    void showViewFitsDialog();

    /** just calls changeNptsDialog->showChangeNptsDialog(0) */
    void showChangeNptsDialog();

    /** just calls graphFileDialog->showGraphFileDialog(0) */
    void showGraphFileDialog();
    
    /** just calls filterListEditor->show() */
//    void showFilterListEditor();

    /** just calls debugDialog->show() */
    void showDebugDialog();
    void showDebugLog(); // opens the log page and scrolls down

    /** creates the plugin manager */
    void showPluginManager();

    /** creates the plugin manager */
    void showExtensionManager();

    /** creates the data wizard */
    void showDataWizardWithFile(const QString &input);
    void showDataWizard();

    //show the quickstart dialog, but only if specified so in the KstSettings
    //otherwise, this function does not do anything.
    void showQuickStartDialog();

    /** calls doc->samplesDown */
    void samplesDown();

    /** calls doc->samplesUp */
    void samplesUp();
    
    /** calls update on all the dialogs */
    void updateVisibleDialogs();
    // is there ever a reason to update hidden dialogs?
    void updateDialogs(bool onlyVisible = true);
    void updateDataDialogs(bool dataManager = true, bool viewManager = true);
    void updateDialogsForWindow();
    void updateDataNotifier();
    void updateDataManager(bool onlyVisible);
    void updateViewManager(bool onlyVisible);
    
    /*** shows doc as modified, updates doc and dialogs */
    void registerDocChange();

    void reload();

    void slotPreferences();

    void EventELOGConfigure();

    void createDebugNotifier();
    void destroyDebugNotifier();

    void emitTimezoneChanged(const QString& tz, int utcOffset);

  signals:
    void timezoneChanged(const QString& tz, int utcOffset);
    void settingsChanged();
    void ELOGConfigure();
    void ELOGSubmitEntry(const QString& strMessage);

  private slots:
    void updateMemoryStatus();
    void doDelayedOpens();

  private:
    static const QString& defaultTag;

    /** the configuration object of the application */
    KConfig *config;

    /** The document */
    KstDoc *doc;

    /* Dialog for viewing the scalar values */
    KstViewScalarsDialogI *viewScalarsDialog;

    /* Dialog for viewing the string values */
    KstViewStringsDialogI *viewStringsDialog;

    /* Dialog for viewing the vector values */
    KstViewVectorsDialogI *viewVectorsDialog;

    /* Dialog for viewing the vector values */
    KstViewMatricesDialogI *viewMatricesDialog;
    
    /* Dialog for viewing the fit values */
    KstViewFitsDialogI *viewFitsDialog;

    /* Dialog for changing the data files from Vectors */
    KstChangeFileDialogI *changeFileDialog;
    
    /* Dialog for choosing curve color per file */
    KstChooseColorDialogI *chooseColorDialog;

    /* Dialog for differentiating between curves */
    KstCurveDifferentiateI *differentiateCurvesDialog;
    
    /* Dialog for changing the Sample ranges for Vectors */
    KstChangeNptsDialogI *changeNptsDialog;

    /* Dialog for exporting to a graphics file */
    KstGraphFileDialogI *graphFileDialog;

    /* Dialog for exporting to a text file */
    VectorSaveDialog *vectorSaveDialog;
    
    /* Dialog for looking at data */
    KstDataManagerI *dataManager;
    KstViewManagerI *viewManager;
    
    /* Dialog for looking at debug information */
    KstDebugDialogI *debugDialog;

    GraphicEditorI *_graphicDialog;
    
    /* Dialog for setting monochrome settings for plots */
    KstMonochromeDialogI *_monochromeDialog; 

    //the quickstart dialog
    KstQuickStartDialogI *_quickStartDialog;

    /* contains the recently used filenames */
    KRecentFilesAction *_recent;

    KToggleAction *StatusBarAction;

    /** The Pause Button */
    KToggleAction *PauseAction;

    /** The Reduce samples Button */
    KAction *SamplesDownAction;
    /** The Increase samples Button */
    KAction *SamplesUpAction;
    /** The 'read data from end of file' Button */
    KAction *SamplesFromEndAction;

    /** TiedZoom Button */
    KAction *_tiedZoomAction;

    KAction *_reloadAction;

    /** Radio Collection: XY, X, Y zoom, Text, Layout */
    KRadioAction *XYZoomAction;
    KRadioAction *XZoomAction;
    KRadioAction *YZoomAction;
    KRadioAction *GfxAction;
    KRadioAction *LayoutAction;

    /* Matrix Edit Action */
    KAction *MatrixDialogAction;
    /* Image Edit Action */
    KAction *ImageDialogAction;
    /* New Plot Action */
    KAction *NewPlotAction;
    /* Plot Edit Action */
    KAction *PlotDialogAction;
    /* Vector Edit Action */
    KAction *VectorDialogAction;
    /* Curve Edit Action */
    KAction *CurveDialogAction;
    /* Cumulative spectral decay Edit Action */
    KAction *CsdDialogAction;
    /* Equation Edit Action */
    KAction *EqDialogAction;
    /* Histogram Edit Action */
    KAction *HsDialogAction;
    /* Power Spectrum Edit Action */
    KAction *PsdDialogAction;
    /* Data Manager Action */
    KAction *DataManagerAction;
    /* View Manager Action */    
    KAction *ViewManagerAction;
    /* Plugin Plot Edit Action */
    KAction *PluginDialogAction;
    /* Vector Save Action */
    KAction *_vectorSaveAction;

    /* View Scalars Action */
    KAction *ViewScalarsDialogAction;
    /* View Strings Action */
    KAction *ViewStringsDialogAction;
    /* View Vectors Action */
    KAction *ViewVectorsDialogAction;
    /* View Matrices Action */
    KAction *ViewMatricesDialogAction;
    /* View Fits Action */
    KAction *ViewFitsDialogAction;

    /* Data mode */
    KToggleAction *DataMode;

    /** Change File Action: brings up the change file dialog box */
    KAction *ChangeFileDialogAction;
    /** Choose Color Action: brings up the choose color dialog box */
    KAction *ChooseColorDialogAction;
    /** Differentiate Curves Action: brings up the differentiate curves dialog box */
    KAction *DifferentiateCurvesDialogAction;
    /** Change npts Action: brings up the change data range dialog box */
    KAction *ChangeNptsDialogAction;
    /** GraphFileDialogAction: Brings up the graphics file export window */
    KAction *GraphFileDialogAction;
    /** PluginManagerAction: Brings up the plugin manager window */
    KAction *PluginManagerAction;
    /** ExtensionManagerAction: Brings up the extension manager window */
    KAction *ExtensionManagerAction;
    /** EventMonitorAction: Brings up the event monitor window */
    KAction *EventMonitorAction;

    KAction *DataWizardAction;

    KAction *DebugDialogAction;

    KAction *fileNewWindow;
    KRecentFilesAction *fileOpenRecent;
    KAction *filePrint;
    KAction *fileOpenNew;
    KAction *fileOpen;
    KAction *fileSave;
    KAction *fileSaveAs;
    KAction *fileQuit;
    KAction *fileKeyBindings;
    KAction *filePreferences;
    KAction *fileCopy;
    KAction *filePaste;

    KToggleAction *_saveData;

    KRadioAction *_gfxLineAction;
    KRadioAction *_gfxRectangleAction;
    KRadioAction *_gfxEllipseAction;
    KRadioAction *_gfxLabelAction;
    KRadioAction *_gfxPictureAction;
    KRadioAction *_gfx2DPlotAction;
    KRadioAction *_gfxArrowAction;

    StatusLabel *_readyBar;
    StatusLabel *_memoryBar;
    StatusLabel *_dataBar;
    KProgress *_progressBar;
    bool _stopping;
    KstIfaceImpl *_dcopIface;
    UpdateThread *_updateThread;
    Kst2DPlotMap *_plotHolderWhileOpeningDocument;
    QTimer _memTimer;
    QString _defaultFont;
    KConfig *_dataSourceConfig;
    QValueList<KstOpen> _openQueue;
    KstGraphicType _graphicType;
    
    KstTopLevelView::ViewMode _viewMode;
    QString _createType;
    QGuardedPtr<KstDebugNotifier> _debugNotifier;
    QGuardedPtr<KstDataNotifier> _dataNotifier;
};

#endif // KST_H
// vim: ts=2 sw=2 et
