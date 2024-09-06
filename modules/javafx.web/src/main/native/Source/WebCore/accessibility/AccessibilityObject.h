/*
 * Copyright (C) 2008, 2009, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nuanti Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "AXCoreObject.h"
#include "AXTextRun.h"
#include "CharacterRange.h"
#include "FloatQuad.h"
#include "LayoutRect.h"
#include "Path.h"
#include <wtf/Forward.h>
#include <wtf/Function.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

#if PLATFORM(COCOA)
#include <wtf/RetainPtr.h>

OBJC_CLASS NSArray;
OBJC_CLASS NSData;
OBJC_CLASS NSMutableAttributedString;
OBJC_CLASS NSString;
OBJC_CLASS NSValue;
OBJC_CLASS NSView;
#endif

namespace WebCore {

class AccessibilityObject;
class IntPoint;
class IntSize;
class ScrollableArea;

bool nodeHasPresentationRole(Node*);

class AccessibilityObject : public AXCoreObject, public CanMakeWeakPtr<AccessibilityObject> {
public:
    virtual ~AccessibilityObject();

    AXID treeID() const final;
    ProcessID processID() const override;
    String dbg() const final;

    // After constructing an AccessibilityObject, it must be given a
    // unique ID, then added to AXObjectCache, and finally init() must
    // be called last.
    virtual void init();

    // Prefer using the dedicated functions over consuming these flag values directly, as the flags can sometimes be uninitialized.
    // Also, the dedicated functions traverse for you if the flags aren't yet initialized.
    // For example, use `hasDocumentRoleAncestor()` instead of `ancestorFlags().contains(AXAncestorFlag::HasDocumentRoleAncestor)`.
    OptionSet<AXAncestorFlag> ancestorFlags() const { return m_ancestorFlags; }

    void addAncestorFlags(const OptionSet<AXAncestorFlag>& flags) { m_ancestorFlags.add(flags); }
    bool ancestorFlagsAreInitialized() const { return m_ancestorFlags.contains(AXAncestorFlag::FlagsInitialized); }
    // Computes the flags that this object matches (no traversal is done).
    OptionSet<AXAncestorFlag> computeAncestorFlags() const;
    // Computes the flags that this object and all ancestors match, traversing all the way to the root.
    OptionSet<AXAncestorFlag> computeAncestorFlagsWithTraversal() const;
    void initializeAncestorFlags(const OptionSet<AXAncestorFlag>&);
    bool hasAncestorMatchingFlag(AXAncestorFlag) const;
    bool matchesAncestorFlag(AXAncestorFlag) const;

    bool hasDirtySubtree() const { return m_subtreeDirty; }

    bool hasDocumentRoleAncestor() const override;
    bool hasWebApplicationAncestor() const override;
    bool isInDescriptionListDetail() const override;
    bool isInDescriptionListTerm() const override;
    bool isInCell() const override;
    bool isInRow() const;

    bool isDetached() const override;

    virtual bool isAccessibilityNodeObject() const { return false; }
    bool isAccessibilityRenderObject() const override { return false; }
    virtual bool isAccessibilityScrollbar() const { return false; }
    virtual bool isAccessibilityScrollViewInstance() const { return false; }
    virtual bool isAccessibilitySVGRoot() const { return false; }
    bool isAccessibilityTableInstance() const override { return false; }
    virtual bool isAccessibilityTableColumnInstance() const { return false; }
    bool isAccessibilityARIAGridInstance() const override { return false; }
    bool isAccessibilityARIAGridRowInstance() const override { return false; }
    bool isAccessibilityARIAGridCellInstance() const override { return false; }
    virtual bool isAccessibilityLabelInstance() const { return false; }
    virtual bool isAccessibilityListBoxInstance() const { return false; }
    bool isAXIsolatedObjectInstance() const override { return false; }

    virtual bool isAttachmentElement() const { return false; }
    bool isLink() const override { return false; }
    bool isSecureField() const override { return false; }
    bool isContainedBySecureField() const;
    bool isNativeTextControl() const override { return false; }
    virtual bool isSearchField() const { return false; }
    bool isListBoxOption() const override { return false; }
    bool isAttachment() const override { return false; }
    bool isMediaTimeline() const { return false; }
    bool isFileUploadButton() const final;
    bool isInputImage() const override { return false; }
    virtual bool isSliderThumb() const { return false; }
    bool isControl() const override { return false; }
    bool isRadioInput() const override { return false; }
    bool isLabel() const { return isAccessibilityLabelInstance() || labelForObjects().size(); }

    bool isList() const override { return false; }
    virtual bool isUnorderedList() const { return false; }
    virtual bool isOrderedList() const { return false; }
    virtual bool isDescriptionList() const { return false; }

    // Table support.
    bool isTable() const override { return false; }
    bool isExposable() const override { return true; }
    bool supportsSelectedRows() const override { return false; }
    AccessibilityChildrenVector columns() override { return AccessibilityChildrenVector(); }
    AccessibilityChildrenVector rows() override { return AccessibilityChildrenVector(); }
    unsigned columnCount() override { return 0; }
    unsigned rowCount() override { return 0; }
    AccessibilityChildrenVector cells() override { return AccessibilityChildrenVector(); }
    AccessibilityObject* cellForColumnAndRow(unsigned, unsigned) override { return nullptr; }
    AccessibilityChildrenVector columnHeaders() override { return AccessibilityChildrenVector(); }
    AccessibilityChildrenVector rowHeaders() override { return AccessibilityChildrenVector(); }
    AccessibilityChildrenVector visibleRows() override { return AccessibilityChildrenVector(); }
    String cellScope() const final { return getAttribute(HTMLNames::scopeAttr); }
    AXCoreObject* headerContainer() override { return nullptr; }
    int axColumnCount() const override { return 0; }
    int axRowCount() const override { return 0; }
    virtual Vector<Vector<AXID>> cellSlots() { return { }; }

    // Table cell support.
    bool isTableCell() const override { return false; }
    bool isExposedTableCell() const override { return false; }
    // Returns the start location and row span of the cell.
    std::pair<unsigned, unsigned> rowIndexRange() const override { return { 0, 1 }; }
    // Returns the start location and column span of the cell.
    std::pair<unsigned, unsigned> columnIndexRange() const override { return { 0, 1 }; }
    int axColumnIndex() const override { return -1; }
    int axRowIndex() const override { return -1; }

    // Table column support.
    bool isTableColumn() const override { return false; }
    unsigned columnIndex() const override { return 0; }
    AXCoreObject* columnHeader() override { return nullptr; }

    // Table row support.
    bool isTableRow() const override { return false; }
    unsigned rowIndex() const override { return 0; }
    bool ignoredByRowAncestor() const;

    // ARIA tree/grid row support.
    bool isARIATreeGridRow() const override { return false; }
    AccessibilityChildrenVector disclosedRows() override; // ARIATreeItem implementation. AccessibilityARIAGridRow overrides this method.
    AXCoreObject* disclosedByRow() const override { return nullptr; }

    bool isFieldset() const override { return false; }
    virtual bool isImageMapLink() const { return false; }
    virtual bool isMenuList() const { return false; }
    virtual bool isMenuListPopup() const { return false; }
    virtual bool isMenuListOption() const { return false; }
    virtual bool isNativeSpinButton() const { return false; }
    AXCoreObject* incrementButton() override { return nullptr; }
    AXCoreObject* decrementButton() override { return nullptr; }
    virtual bool isSpinButtonPart() const { return false; }
    virtual bool isIncrementor() const { return false; }
    bool isMockObject() const override { return false; }
    virtual bool isMediaObject() const { return false; }
    bool isARIATextControl() const;
    bool isNonNativeTextControl() const override;
    bool isRangeControl() const;
    bool isMeter() const final;
    bool isStyleFormatGroup() const;
    bool isFigureElement() const;
    bool isKeyboardFocusable() const override;
    bool isOutput() const;

    bool isChecked() const override { return false; }
    bool isEnabled() const override { return false; }
    bool isSelected() const override;
    bool isTabItemSelected() const;
    bool isFocused() const override { return false; }
    virtual bool isHovered() const { return false; }
    bool isIndeterminate() const override { return false; }
    bool isLoaded() const final;
    bool isMultiSelectable() const override { return false; }
    bool isOffScreen() const override { return false; }
    bool isPressed() const override { return false; }
    InsideLink insideLink() const final;
    bool isRequired() const override { return false; }
    bool supportsRequiredAttribute() const override { return false; }
    bool isExpanded() const override;
    bool isVisible() const override { return !isHidden(); }
    virtual bool isCollapsed() const { return false; }
    void setIsExpanded(bool) override { }
    FloatRect unobscuredContentRect() const override;
    FloatRect relativeFrame() const override;
#if PLATFORM(MAC)
    FloatRect primaryScreenRect() const override;
#endif
    FloatRect convertFrameToSpace(const FloatRect&, AccessibilityConversionSpace) const override;
    HashMap<String, AXEditingStyleValueVariant> resolvedEditingStyles() const override;

    // In a multi-select list, many items can be selected but only one is active at a time.
    bool isSelectedOptionActive() const override { return false; }

    bool hasBoldFont() const override { return false; }
    bool hasItalicFont() const override { return false; }
    bool hasMisspelling() const override;
    std::optional<SimpleRange> misspellingRange(const SimpleRange& start, AccessibilitySearchDirection) const override;
    bool hasPlainText() const override { return false; }
    bool hasSameFont(const AXCoreObject&) const override { return false; }
    bool hasSameFontColor(const AXCoreObject&) const override { return false; }
    bool hasSameStyle(const AXCoreObject&) const override { return false; }
    bool hasUnderline() const override { return false; }
    bool hasHighlighting() const override;
    AXTextMarkerRange textInputMarkedTextMarkerRange() const final;

    bool supportsDatetimeAttribute() const override;
    String datetimeAttributeValue() const override;

    bool canSetFocusAttribute() const override { return false; }
    bool canSetValueAttribute() const override { return false; }
    bool canSetSelectedAttribute() const override { return false; }
    bool canSetSelectedChildren() const override { return false; }

    Element* element() const override;
    Node* node() const override { return nullptr; }
    RenderObject* renderer() const override { return nullptr; }
    const RenderStyle* style() const;

    // Note: computeAccessibilityIsIgnored does not consider whether an object is ignored due to presence of modals.
    // Use accessibilityIsIgnored as the word of law when determining if an object is ignored.
    virtual bool computeAccessibilityIsIgnored() const { return true; }
    bool accessibilityIsIgnored() const override;
    void recomputeIsIgnored();
    AccessibilityObjectInclusion defaultObjectInclusion() const;
    bool accessibilityIsIgnoredByDefault() const;
    bool isARIAHidden() const;

    bool isShowingValidationMessage() const;
    String validationMessage() const;

    unsigned blockquoteLevel() const override;
    unsigned headingLevel() const override { return 0; }
    AccessibilityButtonState checkboxOrRadioValue() const override;
    String valueDescription() const override { return String(); }
    float valueForRange() const override { return 0.0f; }
    float maxValueForRange() const override { return 0.0f; }
    float minValueForRange() const override { return 0.0f; }
    virtual float stepValueForRange() const { return 0.0f; }
    AccessibilityObject* selectedListItem();
    int layoutCount() const override { return 0; }
    double loadingProgress() const final;
    WEBCORE_EXPORT static bool isARIAControl(AccessibilityRole);
    bool supportsCheckedState() const override;

    bool supportsARIARoleDescription() const;
    bool supportsARIAOwns() const override { return false; }
    bool isActiveDescendantOfFocusedContainer() const;

    String popupValue() const override;
    bool hasDatalist() const;
    bool supportsHasPopup() const override;
    bool pressedIsPresent() const override;
    bool ariaIsMultiline() const;
    String invalidStatus() const override;
    bool supportsPressed() const;
    bool supportsExpanded() const override;
    bool supportsChecked() const override;
    bool supportsRowCountChange() const;
    AccessibilitySortDirection sortDirection() const override;
    virtual bool canvasHasFallbackContent() const { return false; }
    bool supportsRangeValue() const override;
    String identifierAttribute() const override;
    String linkRelValue() const override;
    Vector<String> classList() const override;
    AccessibilityCurrentState currentState() const override;
    bool supportsCurrent() const override;
    bool supportsKeyShortcuts() const override;
    String keyShortcuts() const override;

    // This function checks if the object should be ignored when there's a modal dialog displayed.
    virtual bool ignoredFromModalPresence() const;
    bool isModalDescendant(Node*) const;
    bool isModalNode() const override;

    bool supportsSetSize() const override;
    bool supportsPosInSet() const override;
    int setSize() const override;
    int posInSet() const override;

    // ARIA drag and drop
    bool supportsDropping() const override { return false; }
    bool supportsDragging() const override { return false; }
    bool isGrabbed() override { return false; }
    void setARIAGrabbed(bool) override { }
    Vector<String> determineDropEffects() const override { return { }; }

    // Called on the root AX object to return the deepest available element.
    AccessibilityObject* accessibilityHitTest(const IntPoint&) const override { return nullptr; }
    // Called on the AX object after the render tree determines which is the right AccessibilityRenderObject.
    virtual AccessibilityObject* elementAccessibilityHitTest(const IntPoint&) const;

    AccessibilityObject* focusedUIElement() const final;

    virtual AccessibilityObject* firstChild() const { return nullptr; }
    virtual AccessibilityObject* lastChild() const { return nullptr; }
    virtual AccessibilityObject* previousSibling() const { return nullptr; }
    virtual AccessibilityObject* nextSibling() const { return nullptr; }
    virtual AccessibilityObject* nextSiblingUnignored(int limit) const;
    virtual AccessibilityObject* previousSiblingUnignored(int limit) const;
    AccessibilityObject* parentObject() const override { return nullptr; }
    AccessibilityObject* displayContentsParent() const;
    AccessibilityObject* parentObjectUnignored() const override;
    virtual AccessibilityObject* parentObjectIfExists() const { return nullptr; }
    static AccessibilityObject* firstAccessibleObjectFromNode(const Node*);
    void findMatchingObjects(AccessibilitySearchCriteria*, AccessibilityChildrenVector&) override;
    virtual bool isDescendantOfBarrenParent() const { return false; }

    bool isDescendantOfRole(AccessibilityRole) const override;

    // Text selection
    Vector<SimpleRange> findTextRanges(const AccessibilitySearchTextCriteria&) const override;
    Vector<String> performTextOperation(const AccessibilityTextOperation&) override;

    virtual AccessibilityObject* observableObject() const { return nullptr; }
    virtual AccessibilityObject* controlForLabelElement() const { return nullptr; }
    AccessibilityObject* scrollBar(AccessibilityOrientation) override { return nullptr; }
    AXCoreObject* internalLinkElement() const override { return nullptr; }
    AccessibilityChildrenVector radioButtonGroup() const override { return { }; }

    virtual AccessibilityRole ariaRoleAttribute() const { return AccessibilityRole::Unknown; }
    bool hasExplicitGenericRole() const { return ariaRoleAttribute() == AccessibilityRole::Generic; }
    bool hasImplicitGenericRole() const { return roleValue() == AccessibilityRole::Generic && !hasExplicitGenericRole(); }
    bool ariaRoleHasPresentationalChildren() const;
    bool inheritsPresentationalRole() const override { return false; }

    // Accessibility Text
    void accessibilityText(Vector<AccessibilityText>&) const override { };
    // A single method for getting a computed label for an AXObject. It condenses the nuances of accessibilityText. Used by Inspector.
    WEBCORE_EXPORT String computedLabel();

    // A programmatic way to set a name on an AccessibleObject.
    void setAccessibleName(const AtomString&) override { }
    virtual bool hasAttributesRequiredForInclusion() const;

    String title() const override { return { }; }
    String description() const override { return { }; }
    virtual String helpText() const { return { }; }

    std::optional<String> textContent() const override;
    bool hasTextContent() const;
#if PLATFORM(COCOA)
    bool hasAttributedText() const;
#endif

    // Methods for determining accessibility text.
    bool isARIAStaticText() const { return ariaRoleAttribute() == AccessibilityRole::StaticText; }
    String stringValue() const override { return { }; }
    String textUnderElement(AccessibilityTextUnderElementMode = AccessibilityTextUnderElementMode()) const override { return { }; }
    String text() const override { return { }; }
    unsigned textLength() const final;
#if ENABLE(AX_THREAD_TEXT_APIS)
    virtual AXTextRuns textRuns() { return { }; }
    bool hasTextRuns() final { return textRuns().size(); }
#endif
#if PLATFORM(COCOA)
    // Returns an array of strings and AXObject wrappers corresponding to the
    // textruns and replacement nodes included in the given range.
    RetainPtr<NSArray> contentForRange(const SimpleRange&, SpellCheck = SpellCheck::No) const;
    RetainPtr<NSAttributedString> attributedStringForRange(const SimpleRange&, SpellCheck) const;
    RetainPtr<NSAttributedString> attributedStringForTextMarkerRange(AXTextMarkerRange&&, SpellCheck = SpellCheck::No) const override;
#endif
    virtual String ariaLabeledByAttribute() const { return String(); }
    virtual String ariaDescribedByAttribute() const { return String(); }
    const String placeholderValue() const override;
    bool accessibleNameDerivesFromContent() const;
    String brailleLabel() const override { return getAttribute(HTMLNames::aria_braillelabelAttr); }
    String brailleRoleDescription() const override { return getAttribute(HTMLNames::aria_brailleroledescriptionAttr); }
    String embeddedImageDescription() const override;
    std::optional<AccessibilityChildrenVector> imageOverlayElements() override { return std::nullopt; }
    String extendedDescription() const override;

    // Abbreviations
    String expandedTextValue() const override { return String(); }
    bool supportsExpandedTextValue() const override { return false; }

    Vector<Ref<Element>> elementsFromAttribute(const QualifiedName&) const;

    // Only if isColorWell()
    SRGBA<uint8_t> colorValue() const override;

    // FIXME: This should be made final after AccessibilityTable is fixed to use m_role rather than computing its own roleValue().
    AccessibilityRole roleValue() const override { return m_role; }
    virtual AccessibilityRole determineAccessibilityRole() = 0;
    String rolePlatformString() const override;
    String roleDescription() const override;
    String subrolePlatformString() const override;

    AXObjectCache* axObjectCache() const override;

    static AccessibilityObject* anchorElementForNode(Node&);
    static AccessibilityObject* headingElementForNode(Node*);
    virtual Element* anchorElement() const { return nullptr; }
    virtual RefPtr<Element> popoverTargetElement() const { return nullptr; }
    bool supportsPressAction() const override;
    Element* actionElement() const override { return nullptr; }
    virtual LayoutRect boundingBoxRect() const { return { }; }
    LayoutRect elementRect() const override = 0;
#if PLATFORM(COCOA)
    FloatPoint screenRelativePosition() const final;
#else
    FloatPoint screenRelativePosition() const final { return convertFrameToSpace(elementRect(), AccessibilityConversionSpace::Screen).location(); }
#endif
    IntSize size() const final { return snappedIntRect(elementRect()).size(); }
    IntPoint clickPoint() final;
    IntPoint clickPointFromElementRect() const;
    static IntRect boundingBoxForQuads(RenderObject*, const Vector<FloatQuad>&);
    Path elementPath() const override { return Path(); }
    bool supportsPath() const override { return false; }

    TextIteratorBehaviors textIteratorBehaviorForTextRange() const;
    CharacterRange selectedTextRange() const override { return { }; }
    int insertionPointLineNumber() const override { return -1; }

    URL url() const override { return URL(); }
    VisibleSelection selection() const final;
    String selectedText() const override { return String(); }
    String accessKey() const override { return nullAtom(); }
    String localizedActionVerb() const override;
    String actionVerb() const override;

    bool isWidget() const override { return false; }
    Widget* widget() const override { return nullptr; }
    PlatformWidget platformWidget() const override { return nullptr; }
    Widget* widgetForAttachmentView() const override { return nullptr; }
    bool isPlugin() const override { return false; }

#if PLATFORM(COCOA)
    RemoteAXObjectRef remoteParentObject() const override;
    FloatRect convertRectToPlatformSpace(const FloatRect&, AccessibilityConversionSpace) const override;
#endif
    Page* page() const override;
    Document* document() const override;
    LocalFrameView* documentFrameView() const override;
    LocalFrame* frame() const;
    LocalFrame* mainFrame() const;
    Document* topDocument() const;
    RenderView* topRenderer() const;
    ScrollView* scrollView() const override { return nullptr; }
    String language() const override;
    // 1-based, to match the aria-level spec.
    unsigned hierarchicalLevel() const override { return 0; }
    bool isInlineText() const override;

    // Ensures that the view is focused and active before attempting to set focus to an AccessibilityObject.
    // Subclasses that override setFocused should call this base implementation first.
    void setFocused(bool) override;

    void setSelectedText(const String&) override { }
    void setSelectedTextRange(CharacterRange&&) override { }
    bool setValue(const String&) override { return false; }
    void setValueIgnoringResult(const String& value) final { setValue(value); }
    bool replaceTextInRange(const String&, const CharacterRange&) override;
    bool insertText(const String&) override;

    bool setValue(float) override { return false; }
    void setValueIgnoringResult(float value) final { setValue(value); }
    void setSelected(bool) override { }
    void setSelectedRows(AccessibilityChildrenVector&&) final;

    void performDismissActionIgnoringResult() final { performDismissAction(); }
    bool press() override;

    AccessibilityOrientation orientation() const override;
    void increment() override { }
    void decrement() override { }
    virtual bool toggleDetailsAncestor() { return false; }

    virtual void updateRole();
    bool childrenInitialized() const { return m_childrenInitialized; }
    const AccessibilityChildrenVector& children(bool updateChildrenIfNeeded = true) override;
    virtual void addChildren() { }
    enum class DescendIfIgnored : bool { No, Yes };
    void addChild(AXCoreObject*, DescendIfIgnored = DescendIfIgnored::Yes);
    virtual void insertChild(AXCoreObject*, unsigned, DescendIfIgnored = DescendIfIgnored::Yes);
    virtual bool canHaveChildren() const { return true; }
    void updateChildrenIfNecessary() override;
    virtual void setNeedsToUpdateChildren() { }
    virtual void setNeedsToUpdateSubtree() { }
    virtual void clearChildren();
    virtual bool needsToUpdateChildren() const { return false; }
#if PLATFORM(COCOA)
    void detachFromParent() override;
#else
    void detachFromParent() override { }
#endif
    bool isDetachedFromParent() override { return false; }

    AccessibilityChildrenVector selectedChildren() override;
    void setSelectedChildren(const AccessibilityChildrenVector&) override { }
    AccessibilityChildrenVector visibleChildren() override { return { }; }
    bool shouldFocusActiveDescendant() const;

    WEBCORE_EXPORT static AccessibilityRole ariaRoleToWebCoreRole(const String&);
    virtual bool hasAttribute(const QualifiedName&) const;
    virtual const AtomString& getAttribute(const QualifiedName&) const;
    String nameAttribute() const final;
    int getIntegralAttribute(const QualifiedName&) const;
    bool hasTagName(const QualifiedName&) const;
    AtomString tagName() const override;
    bool hasDisplayContents() const;

    std::optional<SimpleRange> simpleRange() const override;
    VisiblePositionRange visiblePositionRange() const override { return { }; }
    AXTextMarkerRange textMarkerRange() const override;

    std::optional<SimpleRange> visibleCharacterRange() const override;
    VisiblePositionRange visiblePositionRangeForLine(unsigned) const override { return VisiblePositionRange(); }

    static bool replacedNodeNeedsCharacter(Node* replacedNode);

    VisiblePositionRange visiblePositionRangeForUnorderedPositions(const VisiblePosition&, const VisiblePosition&) const override;
    VisiblePositionRange positionOfLeftWord(const VisiblePosition&) const override;
    VisiblePositionRange positionOfRightWord(const VisiblePosition&) const override;
    VisiblePositionRange leftLineVisiblePositionRange(const VisiblePosition&) const override;
    VisiblePositionRange rightLineVisiblePositionRange(const VisiblePosition&) const override;
    VisiblePositionRange sentenceForPosition(const VisiblePosition&) const override;
    VisiblePositionRange paragraphForPosition(const VisiblePosition&) const override;
    VisiblePositionRange styleRangeForPosition(const VisiblePosition&) const override;
    VisiblePositionRange visiblePositionRangeForRange(const CharacterRange&) const override;
    VisiblePositionRange lineRangeForPosition(const VisiblePosition&) const override;
    virtual VisiblePositionRange selectedVisiblePositionRange() const { return { }; }

    std::optional<SimpleRange> rangeForCharacterRange(const CharacterRange&) const override;
#if PLATFORM(COCOA)
    AXTextMarkerRange textMarkerRangeForNSRange(const NSRange&) const override;
#endif
#if PLATFORM(MAC)
    AXTextMarkerRange selectedTextMarkerRange() override;
#endif
    static String stringForVisiblePositionRange(const VisiblePositionRange&);
    String stringForRange(const SimpleRange&) const override;
    virtual IntRect boundsForVisiblePositionRange(const VisiblePositionRange&) const { return IntRect(); }
    IntRect boundsForRange(const SimpleRange&) const final;
    void setSelectedVisiblePositionRange(const VisiblePositionRange&) const override { }

    VisiblePosition visiblePositionForPoint(const IntPoint&) const final;
    VisiblePosition nextLineEndPosition(const VisiblePosition&) const override;
    VisiblePosition previousLineStartPosition(const VisiblePosition&) const override;
    VisiblePosition nextSentenceEndPosition(const VisiblePosition&) const override;
    VisiblePosition previousSentenceStartPosition(const VisiblePosition&) const override;
    VisiblePosition nextParagraphEndPosition(const VisiblePosition&) const override;
    VisiblePosition previousParagraphStartPosition(const VisiblePosition&) const override;
    VisiblePosition visiblePositionForIndex(unsigned, bool /*lastIndexOK */) const override { return VisiblePosition(); }

    VisiblePosition visiblePositionForIndex(int) const override { return VisiblePosition(); }
    int indexForVisiblePosition(const VisiblePosition&) const override { return 0; }

    int lineForPosition(const VisiblePosition&) const override;
    CharacterRange plainTextRangeForVisiblePositionRange(const VisiblePositionRange&) const;
    virtual int index(const VisiblePosition&) const { return -1; }

    CharacterRange doAXRangeForLine(unsigned) const override { return { }; }
    CharacterRange characterRangeForPoint(const IntPoint&) const override;
    CharacterRange doAXRangeForIndex(unsigned) const override { return { }; }
    CharacterRange doAXStyleRangeForIndex(unsigned) const override;

    String doAXStringForRange(const CharacterRange&) const override { return { }; }
    IntRect doAXBoundsForRange(const CharacterRange&) const override { return { }; }
    IntRect doAXBoundsForRangeUsingCharacterOffset(const CharacterRange&) const override { return { }; }
    static StringView listMarkerTextForNodeAndPosition(Node*, Position&&);

    unsigned doAXLineForIndex(unsigned) override;

    String computedRoleString() const override;

    virtual String secureFieldValue() const { return String(); }
    bool isValueAutofillAvailable() const override;
    AutoFillButtonType valueAutofillButtonType() const override;

    // Used by an ARIA tree to get all its rows.
    void ariaTreeRows(AccessibilityChildrenVector&) override;

    // ARIA live-region features.
    AccessibilityObject* liveRegionAncestor(bool excludeIfOff = true) const final { return Accessibility::liveRegionAncestor(*this, excludeIfOff); }
    const String liveRegionStatus() const override { return String(); }
    const String liveRegionRelevant() const override { return nullAtom(); }
    bool liveRegionAtomic() const override { return false; }
    bool isBusy() const override { return false; }
    static const String defaultLiveRegionStatusForRole(AccessibilityRole);
    static bool contentEditableAttributeIsEnabled(Element*);
    bool hasContentEditableAttributeSet() const;

    bool supportsReadOnly() const;
    virtual String readOnlyValue() const;

    bool supportsAutoComplete() const;
    String autoCompleteValue() const override;

    bool hasARIAValueNow() const { return hasAttribute(HTMLNames::aria_valuenowAttr); }
    bool supportsARIAAttributes() const;

    // CSS3 Speech properties.
    OptionSet<SpeakAs> speakAsProperty() const;

    // Make this object visible by scrolling as many nested scrollable views as needed.
    void scrollToMakeVisible() const override;
    // Same, but if the whole object can't be made visible, try for this subrect, in local coordinates.
    void scrollToMakeVisibleWithSubFocus(IntRect&&) const override;
    // Scroll this object to a given point in global coordinates of the top-level window.
    void scrollToGlobalPoint(IntPoint&&) const final;

    enum class ScrollByPageDirection { Up, Down, Left, Right };
    bool scrollByPage(ScrollByPageDirection) const;
    IntPoint scrollPosition() const;
    IntSize scrollContentsSize() const;
    IntRect scrollVisibleContentRect() const;
    void scrollToMakeVisible(const ScrollRectToVisibleOptions&) const;

    // All math elements return true for isMathElement().
    bool isMathElement() const override { return false; }
    bool isMathFraction() const override { return false; }
    bool isMathFenced() const override { return false; }
    bool isMathSubscriptSuperscript() const override { return false; }
    bool isMathRow() const override { return false; }
    bool isMathUnderOver() const override { return false; }
    bool isMathRoot() const override { return false; }
    bool isMathSquareRoot() const override { return false; }
    virtual bool isMathText() const { return false; }
    virtual bool isMathNumber() const { return false; }
    virtual bool isMathOperator() const { return false; }
    virtual bool isMathFenceOperator() const { return false; }
    virtual bool isMathSeparatorOperator() const { return false; }
    virtual bool isMathIdentifier() const { return false; }
    bool isMathTable() const override { return false; }
    bool isMathTableRow() const override { return false; }
    bool isMathTableCell() const override { return false; }
    bool isMathMultiscript() const override { return false; }
    bool isMathToken() const override { return false; }
    virtual bool isMathScriptObject(AccessibilityMathScriptObjectType) const { return false; }
    virtual bool isMathMultiscriptObject(AccessibilityMathMultiscriptObjectType) const { return false; }

    // Root components.
    std::optional<AccessibilityChildrenVector> mathRadicand() override { return std::nullopt; }
    AXCoreObject* mathRootIndexObject() override { return nullptr; }

    // Under over components.
    AXCoreObject* mathUnderObject() override { return nullptr; }
    AXCoreObject* mathOverObject() override { return nullptr; }

    // Fraction components.
    AXCoreObject* mathNumeratorObject() override { return nullptr; }
    AXCoreObject* mathDenominatorObject() override { return nullptr; }

    // Subscript/superscript components.
    AXCoreObject* mathBaseObject() override { return nullptr; }
    AXCoreObject* mathSubscriptObject() override { return nullptr; }
    AXCoreObject* mathSuperscriptObject() override { return nullptr; }

    // Fenced components.
    String mathFencedOpenString() const override { return String(); }
    String mathFencedCloseString() const override { return String(); }
    int mathLineThickness() const override { return 0; }
    virtual bool isAnonymousMathOperator() const { return false; }

    // Multiscripts components.
    void mathPrescripts(AccessibilityMathMultiscriptPairs&) override { }
    void mathPostscripts(AccessibilityMathMultiscriptPairs&) override { }

    // Visibility.
    bool isAXHidden() const;
    bool isDOMHidden() const;
    bool isHidden() const { return isAXHidden() || isDOMHidden(); }
    bool isOnScreen() const override;

#if PLATFORM(COCOA)
    void overrideAttachmentParent(AccessibilityObject* parent);
#else
    void overrideAttachmentParent(AccessibilityObject*) { }
#endif

    // A platform-specific method for determining if an attachment is ignored.
    bool accessibilityIgnoreAttachment() const;
    // Gives platforms the opportunity to indicate if an object should be included.
    AccessibilityObjectInclusion accessibilityPlatformIncludesObject() const;

#if PLATFORM(IOS_FAMILY)
    int accessibilitySecureFieldLength() override;
    bool hasTouchEventListener() const override;
#endif

    // allows for an AccessibilityObject to update its render tree or perform
    // other operations update type operations
    void updateBackingStore() override;

#if PLATFORM(COCOA)
    bool preventKeyboardDOMEventDispatch() const override;
    void setPreventKeyboardDOMEventDispatch(bool) override;
    bool fileUploadButtonReturnsValueInTitle() const override;
    String speechHintAttributeValue() const override;
    bool hasApplePDFAnnotationAttribute() const override { return hasAttribute(HTMLNames::x_apple_pdf_annotationAttr); }
#endif

#if PLATFORM(MAC)
    bool caretBrowsingEnabled() const override;
    void setCaretBrowsingEnabled(bool) override;
#endif

    AccessibilityObject* focusableAncestor() override { return Accessibility::focusableAncestor(*this); }
    AccessibilityObject* editableAncestor() override { return Accessibility::editableAncestor(*this); };
    AccessibilityObject* highestEditableAncestor() override { return Accessibility::highestEditableAncestor(*this); }
    AccessibilityObject* exposedTableAncestor(bool includeSelf = false) const final { return Accessibility::exposedTableAncestor(*this, includeSelf); }

    const AccessibilityScrollView* ancestorAccessibilityScrollView(bool includeSelf) const;
    virtual AccessibilityObject* webAreaObject() const { return nullptr; }

    void clearIsIgnoredFromParentData() { m_isIgnoredFromParentData = { }; }
    void setIsIgnoredFromParentDataForChild(AccessibilityObject*);

    AccessibilityChildrenVector documentLinks() override { return AccessibilityChildrenVector(); }

    AccessibilityChildrenVector relatedObjects(AXRelationType) const override;

    String innerHTML() const override;
    String outerHTML() const override;

#if PLATFORM(COCOA) && ENABLE(MODEL_ELEMENT)
    Vector<RetainPtr<id>> modelElementChildren() override;
#endif

#if PLATFORM(IOS_FAMILY)
    struct InlineTextPrediction {
        String text;
        size_t location { 0 };
        void reset()
        {
            text = ""_s;
            location = 0;
        }
    };

    InlineTextPrediction& lastPresentedTextPrediction() { return m_lastPresentedTextPrediction; }
    InlineTextPrediction& lastPresentedTextPredictionComplete() { return m_lastPresentedTextPredictionComplete; }
    void setLastPresentedTextPrediction(Node&, CompositionState, const String&, size_t, bool);
#endif // PLATFORM(IOS_FAMILY)

protected:
    AccessibilityObject() = default;

    // FIXME: Make more of these member functions private.

    void detachRemoteParts(AccessibilityDetachmentType) override;
#if !PLATFORM(JAVA)
    void detachPlatformWrapper(AccessibilityDetachmentType) override;
#endif

    void setIsIgnoredFromParentData(AccessibilityIsIgnoredFromParentData& data) { m_isIgnoredFromParentData = data; }
    bool ignoredFromPresentationalRole() const;

    bool isAccessibilityObject() const override { return true; }

    // If this object itself scrolls, return its ScrollableArea.
    virtual ScrollableArea* getScrollableAreaIfScrollable() const { return nullptr; }
    virtual void scrollTo(const IntPoint&) const { }
    ScrollableArea* scrollableAreaAncestor() const;
    void scrollAreaAndAncestor(std::pair<ScrollableArea*, AccessibilityObject*>&) const;

    virtual bool shouldIgnoreAttributeRole() const { return false; }
    virtual AccessibilityRole buttonRoleType() const;
    String rolePlatformDescription() const;
    bool dispatchTouchEvent();

    static bool isARIAInput(AccessibilityRole);

    AccessibilityObject* radioGroupAncestor() const;

    bool allowsTextRanges() const;
    unsigned getLengthForTextRange() const;

private:
    bool hasAncestorFlag(AXAncestorFlag flag) const { return ancestorFlagsAreInitialized() && m_ancestorFlags.contains(flag); }
    std::optional<SimpleRange> rangeOfStringClosestToRangeInDirection(const SimpleRange&, AccessibilitySearchDirection, const Vector<String>&) const;
    std::optional<SimpleRange> selectionRange() const;
    std::optional<SimpleRange> findTextRange(const Vector<String>& searchStrings, const SimpleRange& start, AccessibilitySearchTextDirection) const;
    std::optional<SimpleRange> visibleCharacterRangeInternal(const std::optional<SimpleRange>&, const FloatRect&, const IntRect&) const;
    Vector<BoundaryPoint> previousLineStartBoundaryPoints(const VisiblePosition&, const SimpleRange&, unsigned) const;
    std::optional<VisiblePosition> previousLineStartPositionInternal(const VisiblePosition&) const;
    bool boundaryPointsContainedInRect(const BoundaryPoint&, const BoundaryPoint&, const FloatRect&) const;
    std::optional<BoundaryPoint> lastBoundaryPointContainedInRect(const Vector<BoundaryPoint>&, const BoundaryPoint&, const FloatRect&, int, int) const;
    std::optional<BoundaryPoint> lastBoundaryPointContainedInRect(const Vector<BoundaryPoint>& boundaryPoints, const BoundaryPoint& startBoundaryPoint, const FloatRect& targetRect) const;

    // Note that "withoutCache" refers to the lack of referencing AXComputedObjectAttributeCache in the function, not the AXObjectCache parameter we pass in here.
    bool accessibilityIsIgnoredWithoutCache(AXObjectCache*) const;
    void setLastKnownIsIgnoredValue(bool);
    void ariaTreeRows(AccessibilityChildrenVector& rows, AccessibilityChildrenVector& ancestors);
    AccessibilityChildrenVector ariaListboxSelectedChildren();
    AccessibilityChildrenVector ariaSelectedRows();

    // Special handling of click point for links.
    IntPoint linkClickPoint();

protected: // FIXME: Make the data members private.
    AccessibilityChildrenVector m_children;
    mutable bool m_childrenInitialized { false };
    AccessibilityRole m_role { AccessibilityRole::Unknown };
private:
    OptionSet<AXAncestorFlag> m_ancestorFlags;
    AccessibilityObjectInclusion m_lastKnownIsIgnoredValue { AccessibilityObjectInclusion::DefaultBehavior };
    // std::nullopt is a valid cached value if this object has no visible characters.
    mutable std::optional<SimpleRange> m_cachedVisibleCharacterRange;
    // This is std::nullopt if we haven't cached any input yet.
    mutable std::optional<std::tuple<std::optional<SimpleRange>, FloatRect, IntRect>> m_cachedVisibleCharacterRangeInputs;
#if PLATFORM(IOS_FAMILY)
    InlineTextPrediction m_lastPresentedTextPrediction;
    InlineTextPrediction m_lastPresentedTextPredictionComplete;
#endif
protected: // FIXME: Make the data members private.
    // FIXME: This can be replaced by AXAncestorFlags.
    AccessibilityIsIgnoredFromParentData m_isIgnoredFromParentData;
    bool m_childrenDirty { false };
    bool m_subtreeDirty { false };
};

inline bool AccessibilityObject::hasDisplayContents() const
{
    RefPtr element = this->element();
    return element && element->hasDisplayContents();
}

inline void AccessibilityObject::recomputeIsIgnored()
{
    // accessibilityIsIgnoredWithoutCache will update m_lastKnownIsIgnoredValue and perform any necessary actions if it has changed.
    accessibilityIsIgnoredWithoutCache(axObjectCache());
}

inline std::optional<BoundaryPoint> AccessibilityObject::lastBoundaryPointContainedInRect(const Vector<BoundaryPoint>& boundaryPoints, const BoundaryPoint& startBoundaryPoint, const FloatRect& targetRect) const
{
    return lastBoundaryPointContainedInRect(boundaryPoints, startBoundaryPoint, targetRect, 0, boundaryPoints.size() - 1);
}

inline VisiblePosition AccessibilityObject::previousLineStartPosition(const VisiblePosition& position) const
{
    return previousLineStartPositionInternal(position).value_or(VisiblePosition());
}

#if !USE(ATSPI)
inline bool AccessibilityObject::allowsTextRanges() const { return true; }
inline unsigned AccessibilityObject::getLengthForTextRange() const { return text().length(); }
#endif

inline bool AccessibilityObject::hasTextContent() const
{
    return isStaticText()
        || roleValue() == AccessibilityRole::WebCoreLink
        || isTextControl() || isTabItem();
}

#if PLATFORM(COCOA)
inline bool AccessibilityObject::hasAttributedText() const
{
    return (isStaticText() && !isARIAStaticText())
        || roleValue() == AccessibilityRole::WebCoreLink
        || isTextControl() || isTabItem();
}
#endif

AccessibilityObject* firstAccessibleObjectFromNode(const Node*, const Function<bool(const AccessibilityObject&)>& isAccessible);

namespace Accessibility {

using PlatformRoleMap = HashMap<AccessibilityRole, String, DefaultHash<unsigned>, WTF::UnsignedWithZeroKeyHashTraits<unsigned>>;

PlatformRoleMap createPlatformRoleMap();
String roleToPlatformString(AccessibilityRole);

} // namespace Accessibility

} // namespace WebCore

#define SPECIALIZE_TYPE_TRAITS_ACCESSIBILITY(ToValueTypeName, predicate) \
SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::ToValueTypeName) \
    static bool isType(const WebCore::AXCoreObject& object) { return object.predicate; } \
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::AccessibilityObject)
static bool isType(const WebCore::AXCoreObject& context) { return context.isAccessibilityObject(); }
SPECIALIZE_TYPE_TRAITS_END()
