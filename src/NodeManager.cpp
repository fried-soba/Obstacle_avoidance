#include "NodeManager.h"
#include <DxLib.h>

NodeManager::NodeManager() {
	flame_cnt = 0;

	//Nodeの領域を動的生成
	for (int column = 0; column < Define::WIN_H; column++) {
		grid[column] = new Node[Define::WIN_W];
	}
	//スタートノード設定
	start = &grid[(int)player.y][(int)player.x];

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calc_hCost(goal);
		}
	}

	//画面端の座標は最初からクローズにしておく
	//横方向
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//横方向
		grid[0][hoge].status = Block;
		grid[Define::WIN_H - 1][hoge].status = Block;
	}
	//縦方向
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		grid[fuga][0].status = Block;
		grid[fuga][Define::WIN_W - 1].status = Block;
	}

	//ブロック内の全てのノードのステータスを進入不可にする
	for (int cnt = 0; cnt < BLOCKS; cnt++) {
		for (int column = block[cnt].y; column < block[cnt].y_end; column++) {
			for (int row = block[cnt].x; row < block[cnt].x_end; row++)
				grid[column][row].status = Block;
		}
	}

	//スタートノードをオープンリストへ
	openList.push(start);
}

//隣接ノードの移動コスト
float NodeManager::moveCost(int x_diff, int y_diff) {
	//縦横のノードは距離1、斜めならルート2
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search() {
	float ng;												//子ノードの合計コスト候補
	int loop_cnt = 0;										//探索のループ回数
	int limit = 40 * 40;									//ループ回数の上限（要調整）
	Node* center = &grid[(int)player.y][(int)player.x];		//探索開始時点の親ノード
	openList.push(center);

	while (!openList.empty()) {
		//オープンリストtopにゴール座標が来る場合は成功
		if (openList.top()->x == goal.x && openList.top()->y == goal.y) {
			printfDx("探索が完了しました\n");
			root.clear();
			getPath(*openList.top());
			return arrival;
		}

		//探索回数が上限に達したら結果を保存して中断
		if (loop_cnt > limit) {
			getPath(*openList.top());
			clearList();
			return unReach;
		}

		//親ノードをオープンリストからpop
		openList.top()->status = Close;
		openList.pop();

		//周辺の8つを子ノードとしてオープンリストに入れる
		for (int diff_row = -1; diff_row <= 1; diff_row++) {
			for (int diff_column = -1; diff_column <= 1; diff_column++) {
				//中央ノードは処理から除外する
				if (!(diff_row == 0 && diff_column == 0)) {
					//子ノードの座標を決定
					Node* child = &grid[center->y + diff_column][center->x + diff_row];
					//子ノードの合計コスト候補の計算
					ng = (center->score - center->h) + child->h + moveCost(diff_row, diff_column);

					//InfluenceMapによる追加コストを計算
					ng += calcIM_cost(child);

					switch (child->status) {
					case None:
						child->status = Open;
						openList.push(child);
						child->parent = center;
						child->score = ng;
						break;
					case Open:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//コストが変更されたので再挿入による再ソート
							openList.pop();
							openList.push(child);
							//優先度付きキューはヒープソートであるため綺麗にソートされてない：要検討課題
						}
						break;
					case Close:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//オープンリストに戻す
							child->status = Open;
							openList.push(child);
						}
						break;
					}
				}
			}
		}
		center = openList.top();
		loop_cnt++;
	}
	printfDx("ルートが見つかりませんでした\n");
	return notFound;
}

void NodeManager::getPath(Node _goal) {
	while (_goal.parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal.x;
		_tmp.y = _goal.y;
		root.push_back(_tmp);
		_goal = *_goal.parent;
	}
	printfDx("経路を取得・格納しました。\n");
	
	//順序が逆順に入ったままなの戻す
	Point _tmp;
	for (int cnt = 0; cnt < root.size() / 2; cnt++) {
		_tmp = root[cnt];
		root[cnt] = root[root.size() - cnt - 1];
		root[root.size() - cnt - 1] = _tmp;
	}
	//子ノードへ移る移動量を親に記録する
	for (int cnt = 0; cnt < root.size() - 2; cnt++) {
		_tmp.x= root[cnt + 1].x - root[cnt].x;
		_tmp.y= root[cnt + 1].y - root[cnt].y;
		player.moveAmount.push_back(_tmp);
	}
	return;
}

//InfluenceMapによるコスト計算
float NodeManager::calcIM_cost(Node* node) {
	//円形領域と三角形領域の危険度
	float Dci = 0, Dtr = 0;
	const float Ctr2 = tanf(20);	//三角形領域の角度を決める定数

	for (int cnt = 0; cnt < HUMAN; cnt++) {
		//人と自機の相対距離の軸成分の距離
		static float dx, dy;
		dx = human[cnt].x - node->x;
		dy = human[cnt].y - node->y;
		//円形領域のコスト
		Dci += Imax * max(0.0, 1 - (pow(dx, 2.0) + pow(dy, 2.0))
			/ (pow((player.radius + human[cnt].radius), 2.0) + pow(Cci, 2.0)));
		//三角形領域のコスト
		static float m = 0.0, s = 0.0;
		static float vx2vy2 = powf(human[cnt].vx, 2.0) + powf(human[cnt].vy, 2.0);
		m = (human[cnt].vx*dx + human[cnt].vy*dy) / vx2vy2;
		s = (-human[cnt].vy*dx + human[cnt].vx*dy) / vx2vy2;
		Dtr += Imax * max(0, 1 - m / Dtr)*max(0, 1 - abs(s) / m * Ctr2);
	}

	return (float)IMw*max(Dci, Dtr);
}

void NodeManager::update() {
	//フレーム同期タイミングの受け渡し
	flame_cnt++;
	flame_cnt %= FREQUENCY;
	player.flameCnt = flame_cnt;
	
	player.update(&goal);
	for (int cnt = 0; cnt < HUMAN; cnt++) {
		human[cnt].update();
		if (player.checkHit(human[cnt].x, human[cnt].y)) {
			player.stop();
			human[cnt].stop();
		}
	}
}

//全てを描画
void NodeManager::draw() {
	goal.draw();
	player.draw();
	for (int cnt = 0; cnt < HUMAN; cnt++)
		human[cnt].draw();
	for (int cnt = 0; cnt < BLOCKS; cnt++)
		block[cnt].draw();

	//探索経路のルート表示
	for (unsigned int cnt = 0; cnt < root.size(); cnt++) {
		int x = root[cnt].x;
		int y = root[cnt].y;
		DrawBox(x, y, ++x, ++y, GetColor(255, 255, 255), FALSE);
	}
	//ゴールまでの距離と現在地を表示
	DrawFormatString(100, 80, GetColor(255, 255, 255), "距離：%.1f x:%.0f y:%.0f", player.distance(goal), player.x, player.y);
}

//リスト中のノードのステータスとコストを初期化して全てpopする
void NodeManager::clearList() {
	Node *tmp;
	while (!openList.empty()) {
		tmp = openList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		openList.pop();
	}
	while (!closeList.empty()) {
		tmp = closeList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		closeList.pop();
	}
}