#include "utils.h"

// Model class for MSE Linear Regression
// Update: https://en.wikipedia.org/wiki/Linear_regression
class LinearRegressionModel{
    // Models Variable
    Dataset data;
    Weights weights;

    // Public function for user
    public:
        // Constructor
        LinearRegressionModel(const Dataset &data_train){
            // Setting Variables
            data = Dataset(data_train);
            weights = Weights(data.number_predictor, 1);
        }

        void print_weights(){
            char function_string[1000];
            strcpy(function_string, "y = ");
            for(int i = 0; i < weights.number_weights; i++){
                char weight[20];
                sprintf(weight,"%.2f * x%d",weights.values[i],i);
                strcat(function_string, weight);
                if(i == weights.number_weights-1){
                    strcat(function_string,"\n");
                }else{
                    strcat(function_string," + ");
                }
            }
            std::cout << function_string;
        }

        // Train the regression model with some data
        void train(int max_iteration, float learning_rate){

            // Mallocating some space for prediction
            float *y_pred = (float *) std::malloc(sizeof(float)*data.length);

            while(max_iteration > 0){
                fit(y_pred);
                weights.update(data, y_pred, learning_rate);

                float mse = mean_squared_error(y_pred,data.y,data.length);

                if(max_iteration % 100 == 0){
                    std::cout << "Iteration left: " << max_iteration << "; MSE = " << mse << "\n";
                }
                max_iteration--;
            }
            free(y_pred);
        }

        float predict(float *x){
            float prediction = 0;
                for(int i = 0; i < weights.number_weights; i++){
                    prediction = prediction + weights.values[i]*x[i];
                }
            return prediction;
        }

    // Private function for algorithm
    private:
        // fit a line given some x and weights
        void fit(float *y_pred){
            
            for(int i = 0; i < data.length; i++){
                y_pred[i] = predict(data.X[i]);
            }
        }
};


int main(){
    // Variable Initialization
    float **X;
    float *y;
    int length_train;
    const char* filename = "test.csv";
    Dataset data = read_csv(filename,&X,&y);

    // Regression Variables
    int max_iteration = 1000;
    float learning_rate = 0.1;

    // Training
    LinearRegressionModel linear_reg = LinearRegressionModel(data);
    linear_reg.train(max_iteration, learning_rate);
    
    // Testing
    float X_test[2];
    X_test[0] = 1;
    X_test[1] = 123;
    float y_test = linear_reg.predict(X_test);
    linear_reg.print_weights();
    std::cout << "Testing for X0 = " << X_test[0] << ", X1 = " << X_test[1];
    std::cout << "y = " << y_test << "\n"; 

}