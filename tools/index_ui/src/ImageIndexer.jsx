import React, { Component } from "react";
import ImageComponent from "./ImageComponent";
import SearchBarComponent from "./SearchBarComponent";
import IndexStatsComponent from "./IndexStatsComponent";
const { ClosestNRequest, ClosestNResponse } = require("./index_pb.js");
const { VisualIndexingServiceClient } = require("./index_grpc_web_pb.js");

class ImageIndexer extends Component {
  constructor(props) {
    super(props);
    this.state = { images: [], query_image: {}, query_set: false };
    this.onSearch = this.onSearch.bind(this);
    this.client = new VisualIndexingServiceClient("http://localhost:8080");
  }

  onSearch(reconstruction_id, image_id) {
    this.setState({
      query_set: true,
      query_image: { reconstruction_id: reconstruction_id, image_id: image_id },
    });

    var request = new ClosestNRequest();
    request.setReconstructionId(reconstruction_id);
    request.setImageId(image_id);
    request.setN(30);
    request.setIncludeDetails(true);

    this.client.closestN(request, {}, (error, response) => {
      console.log(response);
      console.log(error);
      this.setState({
        images: response.getImageIdsList().map((id) => ({
          reconstruction_id: reconstruction_id,
          image_id: id,
          score: response.getDetails().getImageDetailsMap().get(id).getScore(),
          features: response
            .getDetails()
            .getImageDetailsMap()
            .get(id)
            .getDescriptorScoresMap(),
        })),
      });
    });
  }

  render() {
    return (
      <div>
        <SearchBarComponent onSearch={this.onSearch} />
        <br />
        {!this.state.query_set ? (
          ""
        ) : (
          <div>
            <div className="searched-image-panel">
              <ImageComponent
                key={
                  this.state.query_image.image_id +
                  this.state.query_image.reconstruction_id +
                  this.state.query_image.image_id +
                  "query"
                }
                reconstruction_id={this.state.query_image.reconstruction_id}
                image_id={this.state.query_image.image_id}
              ></ImageComponent>
            </div>
            <div className="grid-container">
              {this.state.images.map((e) => (
                <div className="grid-item">
                  <ImageComponent
                    key={
                      this.state.query_image.image_id +
                      e.reconstruction_id +
                      e.image_id
                    }
                    reconstruction_id={e.reconstruction_id}
                    image_id={e.image_id}
                  ></ImageComponent>
                  <IndexStatsComponent
                    key={
                      this.state.query_image.image_id +
                      e.reconstruction_id +
                      e.image_id +
                      "stat"
                    }
                    score={e.score}
                    features={e.features}
                  ></IndexStatsComponent>
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    );
  }
}

export default ImageIndexer;
