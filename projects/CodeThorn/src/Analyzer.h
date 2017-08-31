#ifndef ANALYZER_H
#define ANALYZER_H

/*************************************************************
 * Copyright: (C) 2012 by Markus Schordan                    *
 * Author   : Markus Schordan                                *
 * License  : see file LICENSE in the CodeThorn distribution *
 *************************************************************/

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <sstream>
#include <list>
#include <vector>

#include <omp.h>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#include "Timer.h"
#include "AstTerm.h"
#include "Labeler.h"
#include "CFAnalysis.h"
#include "RoseAst.h"
#include "SgNodeHelper.h"
#include "ExprAnalyzer.h"
#include "EState.h"
#include "TransitionGraph.h"
#include "TransitionGraphReducer.h"
#include "PropertyValueTable.h"
#include "CTIOLabeler.h"
#include "VariableValueMonitor.h"
#include "Solver.h"
#include "AnalysisParameters.h"

// we use INT_MIN, INT_MAX
#include "limits.h"
#include "AstNodeInfo.h"
#include "SgTypeSizeMapping.h"

namespace CodeThorn {

/*! 
  * \author Markus Schordan
  * \date 2012.
 */
  typedef std::list<const EState*> EStateWorkList;
  typedef std::pair<int, const EState*> FailedAssertion;
  typedef std::pair<PState,  std::list<int> > PStatePlusIOHistory;
  enum AnalyzerMode { AM_ALL_STATES, AM_LTL_STATES };

  class SpotConnection;

/*! 
  * \author Markus Schordan
  * \date 2012.
 */
  class Analyzer {
    friend class Solver;
    friend class Solver4;
    friend class Solver5;
    friend class Solver8;
    friend class Solver10;
    friend class Solver11;
    friend class Solver12;
    friend class Visualizer;
    friend class VariableValueMonitor;

  public:
    Analyzer();
    ~Analyzer();

  protected:
    static Sawyer::Message::Facility logger;

  public:
    static void initDiagnostics();
    void initAstNodeInfo(SgNode* node);
    void initializeSolver(std::string functionToStartAt,SgNode* root, bool oneFunctionOnly);
    void initializeTraceSolver(std::string functionToStartAt,SgNode* root);
    void initLabeledAssertNodes(SgProject* root);

    void continueAnalysisFrom(EState* newStartEState);

    void setExplorationMode(ExplorationMode em) { _explorationMode=em; }
    ExplorationMode getExplorationMode() { return _explorationMode; }

    void setSolver(Solver* solver);
    Solver* getSolver();

    //! requires init
    void runSolver();

    long analysisRunTimeInSeconds(); 

    const EState* popWorkList();
    
    // set the size of an element determined by this type
    void setElementSize(VariableId variableId, SgType* elementType);

    EState analyzeVariableDeclaration(SgVariableDeclaration* nextNodeToAnalyze1,EState currentEState, Label targetLabel);

    PState analyzeAssignRhsExpr(PState currentPState,VariableId lhsVar, SgNode* rhs,ConstraintSet& cset);
    // to be replaced by above function
    PState analyzeAssignRhs(PState currentPState,VariableId lhsVar, SgNode* rhs,ConstraintSet& cset);
    
    // thread save; only prints if option status messages is enabled.
    void printStatusMessage(bool);
    void printStatusMessage(string s);
    void printStatusMessageLine(string s);

    void generateAstNodeInfo(SgNode* node);

    // consistency checks
    bool checkEStateSet();
    bool isConsistentEStatePtrSet(std::set<const EState*> estatePtrSet);
    bool checkTransitionGraph();

    //! The analyzer requires a CFAnalysis to obtain the ICFG.
    void setCFAnalyzer(CFAnalysis* cf) { cfanalyzer=cf; }
    CFAnalysis* getCFAnalyzer() const { return cfanalyzer; }

    ExprAnalyzer* getExprAnalyzer();

    // access  functions for computed information
    VariableIdMapping* getVariableIdMapping() { return &variableIdMapping; }
    CTIOLabeler* getLabeler() const;
    Flow* getFlow() { return &flow; }
    PStateSet* getPStateSet() { return &pstateSet; }
    EStateSet* getEStateSet() { return &estateSet; }
    TransitionGraph* getTransitionGraph() { return &transitionGraph; }
    ConstraintSetMaintainer* getConstraintSetMaintainer() { return &constraintSetMaintainer; }
    std::list<FailedAssertion> getFirstAssertionOccurences(){return _firstAssertionOccurences;}

    void setSkipSelectedFunctionCalls(bool defer);
    void setSkipArrayAccesses(bool skip) { exprAnalyzer.setSkipArrayAccesses(skip); }
    bool getSkipArrayAccesses() { return exprAnalyzer.getSkipArrayAccesses(); }

    // specific to the loop-aware exploration modes
    int getIterations() { return _iterations; }
    int getApproximatedIterations() { return _approximated_iterations; }

    // used by the hybrid analyzer (state marshalling)
    void mapGlobalVarInsert(std::string name, int* addr);

    VariableId globalVarIdByName(std::string varName) { return globalVarName2VarIdMapping[varName]; }
    
    typedef std::list<SgVariableDeclaration*> VariableDeclarationList;
    VariableDeclarationList computeUnusedGlobalVariableDeclarationList(SgProject* root);
    VariableDeclarationList computeUsedGlobalVariableDeclarationList(SgProject* root);

    void setStgTraceFileName(std::string filename);
    void setAnalyzerMode(AnalyzerMode am) { _analyzerMode=am; }
    void setMaxTransitions(size_t maxTransitions) { _maxTransitions=maxTransitions; }
    void setMaxIterations(size_t maxIterations) { _maxIterations=maxIterations; }
    void setMaxTransitionsForcedTop(size_t maxTransitions) { _maxTransitionsForcedTop=maxTransitions; }
    void setMaxIterationsForcedTop(size_t maxIterations) { _maxIterationsForcedTop=maxIterations; }
    void setMaxBytes(long int maxBytes) { _maxBytes=maxBytes; }
    void setMaxBytesForcedTop(long int maxBytesForcedTop) { _maxBytesForcedTop=maxBytesForcedTop; }
    void setMaxSeconds(long int maxSeconds) { _maxSeconds=maxSeconds; }
    void setMaxSecondsForcedTop(long int maxSecondsForcedTop) { _maxSecondsForcedTop=maxSecondsForcedTop; }
    void setResourceLimitDiff(int diff) { _resourceLimitDiff=diff; }
    void setDisplayDiff(int diff) { _displayDiff=diff; }
    void setSemanticFoldThreshold(int t) { _semanticFoldThreshold=t; }
    void setNumberOfThreadsToUse(int n) { _numberOfThreadsToUse=n; }
    int getNumberOfThreadsToUse() { return _numberOfThreadsToUse; }
    void setTreatStdErrLikeFailedAssert(bool x) { _treatStdErrLikeFailedAssert=x; }
    void setCompoundIncVarsSet(set<AbstractValue> ciVars);
    void setSmallActivityVarsSet(set<AbstractValue> ciVars);
    void setAssertCondVarsSet(set<AbstractValue> acVars);

    enum GlobalTopifyMode {GTM_IO, GTM_IOCF, GTM_IOCFPTR, GTM_COMPOUNDASSIGN, GTM_FLAGS};
    void setGlobalTopifyMode(GlobalTopifyMode mode);
    void setExternalErrorFunctionName(std::string externalErrorFunctionName);
    // enables external function semantics 
    void enableSVCompFunctionSemantics();
    void disableSVCompFunctionSemantics();
    bool svCompFunctionSemantics() { return _svCompFunctionSemantics; }
    bool stdFunctionSemantics() { return _stdFunctionSemantics; }
    void setModeLTLDriven(bool ltlDriven) { transitionGraph.setModeLTLDriven(ltlDriven); }
    bool getModeLTLDriven() { return transitionGraph.getModeLTLDriven(); }
    // only used in LTL-driven mode
    void setSpotConnection(SpotConnection* connection) { _spotConnection = connection; }
    // only used to initialize solver 10
    void setStartPState(PState startPState) { _startPState=startPState; }

    void setTypeSizeMapping(SgTypeSizeMapping* typeSizeMapping);
    SgTypeSizeMapping* getTypeSizeMapping();

    ///////
    // MJ: 08/24/2017: Currently not used
    //! list of all asserts in a program
    std::list<SgNode*> listOfAssertNodes(SgProject *root);
    size_t memorySizeContentEStateWorkLists();
    static std::string nodeToString(SgNode* node);
    static string lineColSource(SgNode* node);
    SgNode* getCond(SgNode* node);
    set<const EState*> transitionSourceEStateSetOfLabel(Label lab);
    ///////

    // public member variables
    SgNode* startFunRoot;
    PropertyValueTable reachabilityResults;
    boost::unordered_map <std::string,int*> mapGlobalVarAddress;
    boost::unordered_map <int*,std::string> mapAddressGlobalVar;

  private:
    void printStatusMessage(string s, bool newLineFlag);

    std::string analyzerStateToString();

    void addToWorkList(const EState* estate);
    const EState* addToWorkListIfNew(EState estate);
    const EState* takeFromWorkList();
    bool isInWorkList(const EState* estate);
    bool isEmptyWorkList();
    const EState* topWorkList();
    void swapWorkLists();

    /*! if state exists in stateSet, a pointer to the existing state is returned otherwise 
      a new state is entered into stateSet and a pointer to it is returned.
    */
    const PState* processNew(PState& s);
    const PState* processNewOrExisting(PState& s);
    const EState* processNew(EState& s);
    const EState* processNewOrExisting(EState& s);
    const EState* processCompleteNewOrExisting(const EState* es);
    void topifyVariable(PState& pstate, ConstraintSet& cset, AbstractValue varId);
    bool isTopified(EState& s);
    EStateSet::ProcessingResult process(EState& s);
    EStateSet::ProcessingResult process(Label label, PState pstate, ConstraintSet cset, InputOutput io);
    const ConstraintSet* processNewOrExisting(ConstraintSet& cset);
    
    EState createEState(Label label, PState pstate, ConstraintSet cset);
    EState createEState(Label label, PState pstate, ConstraintSet cset, InputOutput io);
    // only used in LTL-driven mode
    void setStartEState(const EState* estate);

    void recordTransition(const EState* sourceEState, Edge e, const EState* targetEState);

    void set_finished(std::vector<bool>& v, bool val);
    bool all_false(std::vector<bool>& v);

    // determines whether lab is a function call label of a function
    // call of the form 'x=f(...)' and returns the varible-id of the
    // lhs, if a valid pointer is provided
    bool isFunctionCallWithAssignment(Label lab,VariableId* varId=0);

    std::list<EState> transferEdgeEState(Edge edge, const EState* estate);
    std::list<EState> transferFunctionCall(Edge edge, const EState* estate);
    std::list<EState> transferFunctionCallLocalEdge(Edge edge, const EState* estate);
    std::list<EState> transferFunctionCallExternal(Edge edge, const EState* estate);
    std::list<EState> transferFunctionCallReturn(Edge edge, const EState* estate);
    std::list<EState> transferFunctionExit(Edge edge, const EState* estate);
    std::list<EState> transferReturnStmt(Edge edge, const EState* estate);
    std::list<EState> transferVariableDeclaration(SgVariableDeclaration* decl,Edge edge, const EState* estate);
    std::list<EState> transferExprStmt(SgNode* nextNodeToAnalyze1, Edge edge, const EState* estate);
    std::list<EState> transferIdentity(Edge edge, const EState* estate);
    std::list<EState> transferAssignOp(SgAssignOp* assignOp, Edge edge, const EState* estate);
    std::list<EState> transferIncDecOp(SgNode* nextNodeToAnalyze2, Edge edge, const EState* estate);
    std::list<EState> transferTrueFalseEdge(SgNode* nextNodeToAnalyze2, Edge edge, const EState* estate);
    std::list<EState> elistify();
    std::list<EState> elistify(EState res);

    std::set<std::string> variableIdsToVariableNames(SPRAY::VariableIdSet);

    bool isStartLabel(Label label);
    int reachabilityAssertCode(const EState* currentEStatePtr);

    bool isFailedAssertEState(const EState* estate);
    bool isVerificationErrorEState(const EState* estate);
    //! adds a specific code to the io-info of an estate which is checked by isFailedAsserEState and determines a failed-assert estate. Note that the actual assert (and its label) is associated with the previous estate (this information can therefore be obtained from a transition-edge in the transition graph).
    EState createFailedAssertEState(const EState estate, Label target);
    EState createVerificationErrorEState(const EState estate, Label target);

    //! list of all asserts in a program
    //! rers-specific error_x: assert(0) version 
    std::list<std::pair<SgLabelStatement*,SgNode*> > listOfLabeledAssertNodes(SgProject *root);
    size_t getNumberOfErrorLabels();
    std::string labelNameOfAssertLabel(Label lab);
    bool isCppLabeledAssertLabel(Label lab);
    std::list<FailedAssertion> _firstAssertionOccurences;

    // functions related to abstractions during the analysis
    void eventGlobalTopifyTurnedOn();
    bool isActiveGlobalTopify();
    bool isIncompleteSTGReady();
    bool isPrecise();

    // specific to the loop-aware exploration modes
    bool isLoopCondLabel(Label lab);
    void incIterations();

    Flow flow;
    CFAnalysis* cfanalyzer;
    std::list<std::pair<SgLabelStatement*,SgNode*> > _assertNodes;
    GlobalTopifyMode _globalTopifyMode;
    set<AbstractValue> _compoundIncVarsSet;
    set<AbstractValue> _smallActivityVarsSet;
    set<AbstractValue> _assertCondVarsSet;
    set<int> _inputVarValues;
    std::list<int> _inputSequence;
    std::list<int>::iterator _inputSequenceIterator;
    ExprAnalyzer exprAnalyzer;
    VariableIdMapping variableIdMapping;
    EStateWorkList* estateWorkListCurrent;
    EStateWorkList* estateWorkListNext;
    EStateWorkList estateWorkListOne;
    EStateWorkList estateWorkListTwo;
    EStateSet estateSet;
    PStateSet pstateSet;
    ConstraintSetMaintainer constraintSetMaintainer;
    TransitionGraph transitionGraph;
    TransitionGraph backupTransitionGraph;
    TransitionGraphReducer _stgReducer;
    int _displayDiff;
    int _resourceLimitDiff;
    int _numberOfThreadsToUse;
    int _semanticFoldThreshold;
    VariableIdMapping::VariableIdSet _variablesToIgnore;
    Solver* _solver;
    AnalyzerMode _analyzerMode;
    set<const EState*> _newNodesToFold;
    long int _maxTransitions;
    long int _maxIterations;
    long int _maxBytes;
    long int _maxSeconds;
    long int _maxTransitionsForcedTop;
    long int _maxIterationsForcedTop;
    long int _maxBytesForcedTop;
    long int _maxSecondsForcedTop;
    // only used in LTL-driven mode
    size_t _prevStateSetSizeDisplay = 0;
    size_t _prevStateSetSizeResource = 0;
    PState _startPState;
    
    VariableValueMonitor variableValueMonitor;

    bool _treatStdErrLikeFailedAssert;
    bool _skipSelectedFunctionCalls;
    ExplorationMode _explorationMode;
    bool _topifyModeActive;
    bool _explicitArrays;

    int _iterations;
    int _approximated_iterations;
    int _curr_iteration_cnt;
    int _next_iteration_cnt;

    bool _stdFunctionSemantics=true;

    bool _svCompFunctionSemantics;
    string _externalErrorFunctionName; // the call of this function causes termination of analysis
    string _externalNonDetIntFunctionName;
    string _externalNonDetLongFunctionName;
    string _externalExitFunctionName;

    std::string _stg_trace_filename;

    Timer _analysisTimer;
    bool _timerRunning = false;

    // =======================================================================
    // ========================== LTLAnalyzer ================================
    // =======================================================================
  public:

    ///////
    // MJ: 08/30/2017: Currently not used
    bool isTerminationRelevantLabel(Label label);
    int numberOfInputVarValues() { return _inputVarValues.size(); }
    ///////

    bool isLTLRelevantEState(const EState* estate);
    bool isLTLRelevantLabel(Label label);
    bool isStdIOLabel(Label label);
    std::set<const EState*> nonLTLRelevantEStates();

    // reduces all states different to stdin and stdout.
    void stdIOFoldingOfTransitionGraph();
    void semanticFoldingOfTransitionGraph();

    // erases transitions that lead directly from one output state to another output state
    void removeOutputOutputTransitions();
    // erases transitions that lead directly from one input state to another input state
    void removeInputInputTransitions();
    // cuts off all paths in the transition graph that lead to leaves 
    // (recursively until only paths of infinite length remain)
    void pruneLeaves();
    // reductions based on a nested BFS from the STG's start state
    void reduceStgToInOutStates();
    void reduceStgToInOutAssertStates();
    void reduceStgToInOutAssertErrStates();
    void reduceStgToInOutAssertWorklistStates();
    // reduction based on all states, works also for disconnected STGs (used by CEGPRA)
    void reduceToObservableBehavior();
    // extracts input sequences leading to each discovered failing assertion where discovered for the first time.
    // stores results in PropertyValueTable "reachabilityResults".
    // returns length of the longest of these sequences if it can be guaranteed that all processed traces are the
    // shortest ones leading to the individual failing assertion (returns -1 otherwise).
    int extractAssertionTraces();

    // LTLAnalyzer
  private:

    // adds a string representation of the shortest input path from start state to assertEState to reachabilityResults. returns the length of the 
    // counterexample input sequence.
    int addCounterexample(int assertCode, const EState* assertEState);
    // returns a list of EStates from source to target. Target has to come before source in the STG (reversed trace). 
    std::list<const EState*>reverseInOutSequenceBreadthFirst(const EState* source, const EState* target, bool counterexampleWithOutput = false);
    // returns a list of EStates from source to target (shortest input path). 
    // please note: target has to be a predecessor of source (reversed trace)
    std::list<const EState*> reverseInOutSequenceDijkstra(const EState* source, const EState* target, bool counterexampleWithOutput = false);
    std::list<const EState*> filterStdInOutOnly(std::list<const EState*>& states, bool counterexampleWithOutput = false) const;
    std::string reversedInOutRunToString(std::list<const EState*>& run);
    //returns the shortest possible number of input states on the path leading to "target".
    int inputSequenceLength(const EState* target);

  public:
    //stores a backup of the created transitionGraph
    void storeStgBackup();
    //load previous backup of the transitionGraph, storing the current version as a backup instead
    void swapStgWithBackup();
    //solver 8 becomes the active solver used by the analyzer. Deletion of previous data iff "resetAnalyzerData" is set to true.
    void setAnalyzerToSolver8(EState* startEState, bool resetAnalyzerData);

    // first: list of new states (worklist), second: set of found existing states
    typedef pair<EStateWorkList,std::set<const EState*> > SubSolverResultType;
    SubSolverResultType subSolver(const EState* currentEStatePtr);

    void insertInputVarValue(int i) { _inputVarValues.insert(i); }
    void addInputSequenceValue(int i) { _inputSequence.push_back(i); }
    void resetToEmptyInputSequence() { _inputSequence.clear(); }
    void resetInputSequenceIterator() { _inputSequenceIterator=_inputSequence.begin(); }
    const EState* getEstateBeforeMissingInput() {return _estateBeforeMissingInput;}
    const EState* getLatestErrorEState() {return _latestErrorEState;}
    std::set<int> getInputVarValues() { return _inputVarValues; }
 public:
    // only used temporarily for binary-binding prototype
    std::map<std::string,VariableId> globalVarName2VarIdMapping;
    std::vector<bool> binaryBindingAssert;

  private:
    const EState* _estateBeforeMissingInput;
    const EState* _latestOutputEState;
    const EState* _latestErrorEState;
    // only used in LTL-driven mode
    SpotConnection* _spotConnection = nullptr;

  }; // end of class Analyzer

} // end of namespace CodeThorn

#include "RersSpecialization.h"

#endif
