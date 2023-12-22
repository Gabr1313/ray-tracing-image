type Matrix = Vec<Vec<f32>>;
type Vector = Vec<f32>;

const EPS: f32 = 1e-5;

pub fn gauss(a: &mut Matrix) -> Option<Vector> {
    let n = a.len();
    let m = a[0].len() - 1;
    let mut wheree = vec![usize::MAX; m];
    let mut col = 0;
    let mut row = 0;
    while col < m && row < n {
        let mut sel = row;
        for i in row..n {
            if a[i][col].abs() > a[sel][col].abs() {
                sel = i;
            }
        }
        if a[sel][col].abs() < EPS {
            col += 1;
            continue;
        }
        for i in col..=m {
            let tmp = a[sel][i];
            a[sel][i] = a[row][i];
            a[row][i] = tmp;
        }
        wheree[col] = row;

        for i in 0..n {
            if i != row {
                let c = a[i][col] / a[row][col];
                for j in col..=m {
                    a[i][j] -= a[row][j] * c;
                }
            }
        }
        col += 1;
        row += 1;
    }
    let mut ans = vec![0.0; m];
    for i in 0..m {
        if wheree[i] != usize::MAX {
            ans[i] = a[wheree[i]][m] / a[wheree[i]][i];
            if ans[i].abs() < EPS {
                ans[i] = 0.0;
            }
        }
    }
    for i in 0..n {
        let mut sum = 0.0;
        for j in 0..m {
            sum += ans[j] * a[i][j];
        }
        if (sum - a[i][m]).abs() > EPS {
            // no solution
            return None;
        }
    }
    for i in 0..m {
        if wheree[i] == usize::MAX {
            // infinite solution
            return None;
        }
    }
    return Some(ans);
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn gaussian_elimination_solution() {
        let mut m = vec![
            vec![1.0, 2.0, 3.0, 1.0],
            vec![4.0, 5.0, 6.0, 1.0],
            vec![1.0, 7.0, 8.0, 1.0],
        ];
        let sol = gauss(&mut m).unwrap();
        assert!(sol[0] < EPS);
        assert!(sol[1] + 1.0 < EPS);
        assert!(sol[2] - 1.0 < EPS);
    }

    #[test]
    fn gaussian_elimination_infinite_solution() {
        let mut m = vec![
            vec![1.0, 2.0, 3.0, 1.0],
            vec![1.0, 2.0, 3.0, 1.0],
            vec![1.0, 7.0, 8.0, 1.0],
        ];
        let sol = gauss(&mut m);
        assert!(sol.is_none());
    }

    #[test]
    fn gaussian_elimination_no_solution() {
        let mut m = vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
            vec![1.0, 7.0, 8.0],
        ];
        let sol = gauss(&mut m);
        assert!(sol.is_none());
    }
}
