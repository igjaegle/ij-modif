// $Id$
//
//    File: DParticleCombo_factory_PreKinFit.h
// Created: Tue Aug  9 14:29:24 EST 2011
// Creator: pmatt (on Linux ifarml6 2.6.18-128.el5 x86_64)
//

#ifndef _DParticleCombo_factory_PreKinFit_
#define _DParticleCombo_factory_PreKinFit_

#include <deque>
#include <vector>
#include <map>

#include "JANA/JFactory.h"
#include "JANA/JObject.h"

#include "particleType.h"
#include "PID/DChargedTrackHypothesis.h"
#include "PID/DNeutralParticleHypothesis.h"
#include "PID/DKinematicData.h"
#include "PID/DChargedTrack.h"
#include "PID/DBeamPhoton.h"
#include "PID/DNeutralShower.h"
#include "PID/DEventRFBunch.h"
#include <HDGEOMETRY/DGeometry.h>
#include <DANA/DApplication.h>

#include "ANALYSIS/DParticleCombo.h"
#include "ANALYSIS/DParticleComboStep.h"
#include "ANALYSIS/DParticleComboBlueprint.h"
#include "ANALYSIS/DAnalysisUtilities.h"

class DParticleCombo_factory_PreKinFit : public jana::JFactory<DParticleCombo>
{
	public:
		DParticleCombo_factory_PreKinFit(){};
		~DParticleCombo_factory_PreKinFit(){};
		const char* Tag(void){return "PreKinFit";}

		void Reset_Pools(void);

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *locEventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *locEventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		const DKinematicData* Get_DetectedParticle(const DReaction* locReaction, const DEventRFBunch* locEventRFBunch, const DParticleComboBlueprintStep* locParticleComboBlueprintStep, size_t locParticleIndex, vector<const DChargedTrackHypothesis*>& locChargedTrackHypotheses, vector<const DNeutralParticleHypothesis*>& locNeutralParticleHypotheses);
		DKinematicData* Create_Target(Particle_t locPID);

		bool Cut_CombinedPIDFOM(const DParticleCombo* locParticleCombo) const;
		bool Cut_CombinedTrackingFOM(const DParticleCombo* locParticleCombo) const;
		bool Cut_PIDFOM(const DReaction* locReaction, const DChargedTrackHypothesis* locChargedTrackHypothesis) const;
		bool Cut_PIDFOM(const DReaction* locReaction, const DNeutralParticleHypothesis* locNeutralParticleHypothesis) const;
		bool Cut_HasDetectorMatch(const DReaction* locReaction, const DChargedTrackHypothesis* locChargedTrackHypothesis) const;

		void Calc_CommonSpacetimeVertices(DParticleCombo* locParticleCombo) const;
		void Setup_VertexConstraint(DParticleCombo* locParticleCombo, size_t locStepIndex, deque<const DKinematicData*>& locDetectedVertexParticles, deque<const DKinematicData*>& locDetectedTimeParticles, deque<size_t>& locIncludedStepIndices) const;

		DParticleComboStep* Clone_ParticleComboStep(const DParticleComboStep* locParticleComboStep);
		void Reset_KinematicData(DKinematicData* locKinematicData);
		DParticleComboStep* Get_ParticleComboStepResource(void);
		DKinematicData* Get_KinematicDataResource(void);
		DBeamPhoton* Get_BeamPhotonResource(void);

		deque<DParticleComboStep*> dParticleComboStepPool_All;
		deque<DParticleComboStep*> dParticleComboStepPool_Available;

		deque<DKinematicData*> dKinematicDataPool_All;
		deque<DKinematicData*> dKinematicDataPool_Available;

		map<const DParticleComboBlueprintStep*, const DParticleComboStep*> dComboBlueprintStepMap;

		size_t MAX_DParticleComboStepPoolSize;
		size_t MAX_DKinematicDataPoolSize;
		size_t MAX_DBeamPhotonPoolSize;

		// PRE-DPARTICLECOMBO CUT VALUES
			//bool = true/false for cut enabled/disabled, double = cut value
			//Command-line values will override these values
		pair<bool, double> dMinChargedPIDFOM; //the minimum PID FOM for a particle used for this DReaction
		pair<bool, double> dMinPhotonPIDFOM; //the minimum PID FOM for a neutral particle used for this DReaction
		pair<bool, double> dMinCombinedPIDFOM; //the minimum combined PID FOM for all charged tracks used for this DReaction
		pair<bool, double> dMinCombinedTrackingFOM; //the minimum combined Tracking FOM for all charged tracks used for this DReaction
		pair<bool, double> dMaxPhotonRFDeltaT; //the maximum photon-rf time difference: used for photon selection
		pair<bool, bool> dHasDetectorMatchFlag; //if both are true, require tracks to have a detector match

		double dTargetCenterZ;
		const DAnalysisUtilities* dAnalysisUtilities;
};

#endif // _DParticleCombo_factory_PreKinFit_

