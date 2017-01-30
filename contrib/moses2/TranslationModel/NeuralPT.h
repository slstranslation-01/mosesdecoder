#pragma once
#include <unordered_map>
#include "StatefulPhraseTable.h"
#include "../legacy/Range.h"

namespace amunmt
{
 class MosesPlugin;
 class Vocab;
}

namespace Moses2
{
class NeuralPTState;

struct AmunPhrase
{
  std::vector<const Factor*> words;
  SCORE score;
  Range range;
};

class NeuralPT : public StatefulPhraseTable
{
public:
  NeuralPT(size_t startInd, const std::string &line);

  virtual void Load(System &system);

  void SetParameter(const std::string& key, const std::string& value);

  virtual void
  EvaluateInIsolation(MemPool &pool, const System &system, const Phrase<Moses2::Word> &source,
      const TargetPhraseImpl &targetPhrase, Scores &scores,
      SCORE &estimatedScore) const {}

  virtual void
  EvaluateInIsolation(MemPool &pool, const System &system, const Phrase<SCFG::Word> &source,
      const TargetPhrase<SCFG::Word> &targetPhrase, Scores &scores,
      SCORE &estimatedScore) const {}

  //! return uninitialise state
  virtual FFState* BlankState(MemPool &pool, const System &sys) const;

  //! return the state associated with the empty hypothesis for a given sentence
  virtual void EmptyHypothesisState(FFState &state, const ManagerBase &mgr,
      const InputType &input, const Hypothesis &hypo) const;

  virtual void EvaluateWhenApplied(const ManagerBase &mgr,
      const Hypothesis &hypo, const FFState &prevState, Scores &scores,
      FFState &state) const;

  virtual void EvaluateWhenApplied(const SCFG::Manager &mgr,
      const SCFG::Hypothesis &hypo, int featureID, Scores &scores,
      FFState &state) const;

  virtual void EvaluateBeforeExtending(const Hypotheses &hypos, const Manager &mgr) const;

protected:
  size_t m_factor;

  std::string m_modelPath;
  size_t m_maxDevices;
  amunmt::MosesPlugin *m_plugin;

  typedef std::vector<const Factor*> VocabAmun2Moses;
  typedef std::unordered_map<const Factor*, size_t> VocabMoses2Amun;
  VocabAmun2Moses m_sourceA2M, m_targetA2M;
  VocabMoses2Amun m_sourceM2A, m_targetM2A;

  void EvaluateBeforeExtending(Hypothesis &hypo, const Manager &mgr) const;
  std::vector<AmunPhrase> Lookup(const NeuralPTState &prevState) const;

  void CreateVocabMapping(
      System &system,
      const amunmt::Vocab &vocab,
      VocabAmun2Moses &a2m,
      VocabMoses2Amun &m2a) const;
};

}
