

// Gradient Descent

class GradientDescent{
    public:
    float learning_rate = 0.99;

    float iterate(float& parameters, Functor& cost_gradient ){

        parameters = parameters - learning_rate*cost_gradient(parameters);
        return parameters;
    }
};

class GradientDescentX{
    public:
    float learning_rate = 0.99;

    float iterate(float& parameters, float& cost_gradient ){
        parameters = parameters - learning_rate*cost_gradient();
        return parameters;
    }
};


class MomentumGradientDescent : GradientDescent{
    private:
    float velocity = 0;

    public:
    float iterate(float& parameters, float& cost_gradient ){
        velocity = velocity - learning_rate*cost_gradient;
        parameters = parameters - velocity;
        return parameters;
    }
};


// Nesterov accelerated gradient
class NAG: MomentumGradientDescent{
    public:
    float iterate(float& parameters, float& cost_gradient ){
        velocity = velocity - learning_rate*cost_gradient; // no good. need to review
        parameters = parameters - velocity;
        return parameters;
    }   
};


class Adam : GradientDescent{
    private:
    const float beta1 = 0.9;
    const float beta2 = 0.999;
    const float epsilon = 0.00000001; // 10^-8

    float moment = 0;
    float velocity = 0;

    public:
    float iterate(float& parameters, float& cost_gradient){
        moment = beta1*moment + (1-beta2)*cost_gradient;
        velocity = beta2*velocity + (1-beta1)*cost_gradient*cost_gradient;

        float moment_estim = moment/(1-beta1);
        float velocity_estim = velocity/(1-beta2);

        parameters = parameters - learning_rate/(math.sqrt(velocity_estim) + epsilon)*moment_estim;
    }
};


class Nadam : Adam {
    //TODO
    public:
    float iterate(float& parameters, float& cost_gradient){
        float momentum = gamma*momentum + learning_rate*cost_gradient;
        parameters = parameters - momentum;
        return parameters;
    }

};



class AMSGrad : Adam {
    private:
    float velocity_estim = 0;
    public:
    float iterate(float& parameters, float& cost_gradient){
        moment = beta1*moment + (1-beta2)*cost_gradient;
        velocity = beta2*velocity + (1-beta1)*cost_gradient*cost_gradient;

        float moment_estim = moment/(1-beta1);
        velocity_estim = velocity > velocity_estim ? velocity : velocity_estim ;

        parameters = parameters - learning_rate/(math.sqrt(velocity_estim) + epsilon)*moment_estim;
    }
};
