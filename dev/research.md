Working Hypotheses
==================

Cortical Learning Algorithm
---------------------------

### Autoencoder / Knowledgebase

*22 August 2021* - 
* determine how to use autoencoded layers in processing of an independent/new (eg higher or lower layer) subnet
    * not every cortical circuit is necessarily an autoencoder; some circuits map a set of inputs to a set of ideal (supervised) outputs
	    * subnet processing depth is limited, in that only the final layer of a circuit can be trained using a hebbian learning model

*21 August 2021* - 
* Verify autoencoder cortical learning algorithm simulation is hebbian (equivalent to final layer backpropagation): 
	* network parameter adjustments;
    	* error_L = (y_L - A_L)
		* dC/dB = error_l
    	* dC/dW = A_l-1 * error_l
    	* Bnew = B+dC/dB
    	* Wnew = W+dC/dW

*20 August 2021* - 
* Implement and test autoencoder cortical learning algorithm simulation
    * create autoencoder-corticalLearningAlgorithmSimulation.ipynb based on autoencoder.ipynb
    * simulateCorticalCircuit: onlyTrainFinalLayer = True
	
*18 August 2021* - 
* A naive examination of the local information flow through the cortical layers (from L4) suggests an autoencoder. 
    * [OLD] Those pathways/synapses that result in positive feedback L4->L1->L4 (and subcircuits) get reinforced, such that when a similar input dataset is presented, similar pathways (that efficiently encode the dataset) get activated.

*23 July 2021* - 
* A preliminary review of the local connectome directionality/flow suggests a possible cortical learning algorithm autoencoder or knowledgebase.

*19 July 2021* - 
* information appears to be going both directions from L4 entry;
    * perhaps a knowledge base
    * L4->L2->L4 might be an autoencoder
        * autoencoders do not require supervised output values
        * how to send back the error signal?
            * strengthen activated connections
            * [OLD] successful propagation tree from L4->L2[->L3]->L4 fires L4 again which sends signal back to L2 reinforcing the pathways
                * those pathways which are sequentially/recursively activated [multiple iterations through] get reinforced
					* [all other pathways are suppressed]
					* pathways which are not sequentially/recursively reinforced are suppressed
			* this may not be necessary;
				* eg if L4 continuously fires irrespective of L2->L4 autoencoding circutry
					* apply hebbian learning to those incoming synapses which are activated  
			* [OLD] absent (0) inputs might be clamped at 0, preventing recursive feedback
			* the only synapses to learn are between L2 and L4 
				* L4->L2 connections could just be random (eg SDR / k winners takes all / etc)
			* [OLD] but what about the L2/L4->L4 synapses that do not connect to the neuron being autoencoded, are these reinforced also?
				* only reinforce the L2/L3->L4 synapses that contribute to the activation of L4
					* this will automatically/naturally occur (only those synapses connected to an activated output neuron will be reinforced)
			* learning model is a hebbian learning model
