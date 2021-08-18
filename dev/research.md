Working Hypotheses
==================

Cortical Learning Algorithm
---------------------------

### Autoencoder / Knowledgebase

*18 August 2021* - 
* A naive examination of the local information flow through the cortical layers (from L4) suggests an autoencoder. Those pathways/synapses that result in positive feedback L4->L1->L4 (and subcircuits) get reinforced, such that when a similar input dataset is presented, similar pathways (that efficiently encode the dataset) get activated.

*23 July 2021* - 
* A preliminary review of the local connectome directionality/flow suggests a possible cortical learning algorithm autoencoder or knowledgebase.

*19 July 2021* - 
* information appears to be going both directions from L4 entry;
    * perhaps a knowledge base
    * L4->L2->L4 might be an autoencoder
        * autoencoders do not require supervised output values
        * how to send back the error signal?
            * strengthen activated connections
            * successful propagation tree from L4->L2[->L3]->L4 fires L4 again which sends signal back to L2 reinforcing the pathways
                * those pathways which are sequentially/recursively activated [multiple iterations through] get reinforced
                
