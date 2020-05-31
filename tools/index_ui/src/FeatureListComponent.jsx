import React, { Component } from "react";

class FeatureListComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      features: props.features,
      onFeatureSelect: props.onFeatureSelect,
    };
    this.featureSelected = this.featureSelected.bind(this);
  }

  featureSelected(event) {}
  render() {
    return (
      <div className="feature-list">
        {this.state.features.map((e) => {
          const feature = e.feature;
          const score = e.score;
          return <div className="feautre-row">
            <div className="feature-name">{feature}</div>
            <div className="feature-score">{score}</div>
          </div>;
        })}
      </div>
    );
  }
}

export default FeatureListComponent;
