type state = {repoData: option(array(RepoData.repo))};

type action =
  | Loaded(array(RepoData.repo));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  reducer: (action, _state) =>
    switch action {
    | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
    },
  didMount: self => {
    let handleReposLoaded = repoData => self.send(Loaded(repoData));
    RepoData.fetchRepos()
    |> Js.Promise.then_(repoData => {
         handleReposLoaded(repoData);
         Js.Promise.resolve();
       })
    |> ignore;
  },
  render: self => {
    let repoItem =
      switch self.state.repoData {
      | Some(repos) =>
        ReasonReact.array(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
            repos
          )
        )
      | None => ReasonReact.string("Loading")
      };
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      repoItem
    </div>;
  }
};
